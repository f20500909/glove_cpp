#!/usr/bin/env python


from __future__ import absolute_import
from __future__ import division
from __future__ import print_function
from __future__ import unicode_literals

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import sys
import setuptools
import os
import subprocess
import platform

__version__ = '0.2.3'
SRC = "src"


# Based on https://github.com/pybind/python_example

class get_pybind_include(object):
    """Helper class to determine the pybind11 include path

    The purpose of this class is to postpone importing pybind11
    until it is actually installed, so that the ``get_include()``
    method can be invoked. """

    def __init__(self, user=False):
        try:
            import pybind11
        except ImportError:
            if subprocess.call([sys.executable, '-m', 'pip', 'install', 'pybind11']):
                raise RuntimeError('pybind11 install failed.')

        self.user = user

    def __str__(self):
        import pybind11
        return pybind11.get_include(self.user)


try:
    coverage_index = sys.argv.index('--coverage')
except ValueError:
    coverage = False
else:
    del sys.argv[coverage_index]
    coverage = True

src_files = map(str, os.listdir(SRC))
src_cpp = list(filter(lambda x: x.endswith('.cpp'), src_files))

src_cpp = list(
    map(lambda x: str(os.path.join(SRC, x)), src_cpp)
)

print(get_pybind_include())
print(get_pybind_include(user=True))
src_cpp.append('src/glove/py_bind/glove_pybind.cpp')
print(src_cpp)
ext_modules = [
    Extension(
        "glove_pybind",
        # Path to pybind11 headers
        src_cpp,
        include_dirs=[
            get_pybind_include(),
            get_pybind_include(user=True),
            # Path to fasttext source code
            SRC,
        ],
        language='c++',
        extra_compile_args=["-std=c++11"],
    ),
]


# As of Python 3.6, CCompiler has a `has_flag` method.
# cf http://bugs.python.org/issue26689
def has_flag(compiler, flags):
    """Return a boolean indicating whether a flag name is supported on
    the specified compiler.
    """
    import tempfile
    with tempfile.NamedTemporaryFile('w', suffix='.cpp') as f:
        f.write('int main (int argc, char **argv) { return 0; }')
        try:
            compiler.compile([f.name], extra_postargs=flags)
        except setuptools.distutils.errors.CompileError:
            return False
    return True


def cpp_flag(compiler):
    """Return the -std=c++[0x/11/14] compiler flag.
    The c++14 is preferred over c++0x/11 (when it is available).
    """
    standards = ['-std=c++14', '-std=c++11', '-std=c++0x']
    for standard in standards:
        if has_flag(compiler, [standard]):
            return standard
    raise RuntimeError(
        'Unsupported compiler -- at least C++0x support '
        'is needed!'
    )


class BuildExt(build_ext):
    """A custom build extension for adding compiler-specific options."""
    c_opts = {
        'msvc': ['/EHsc'],
        'unix': [],
    }

    def build_extensions(self):
        if sys.platform == 'darwin':
            mac_osx_version = float('.'.join(platform.mac_ver()[0].split('.')[:2]))
            os.environ['MACOSX_DEPLOYMENT_TARGET'] = str(mac_osx_version)
            all_flags = ['-stdlib=libc++', '-mmacosx-version-min=10.7']
            if has_flag(self.compiler, [all_flags[0]]):
                self.c_opts['unix'] += [all_flags[0]]
            elif has_flag(self.compiler, all_flags):
                self.c_opts['unix'] += all_flags
            else:
                raise RuntimeError(
                    'libc++ is needed! Failed to compile with {} and {}.'.
                        format(" ".join(all_flags), all_flags[0])
                )
        ct = self.compiler.compiler_type
        opts = self.c_opts.get(ct, [])
        extra_link_args = []

        if coverage:
            coverage_option = '--coverage'
            opts.append(coverage_option)
            extra_link_args.append(coverage_option)

        if ct == 'unix':
            opts.append('-DVERSION_INFO="%s"' % self.distribution.get_version())
            opts.append(cpp_flag(self.compiler))
            if has_flag(self.compiler, ['-fvisibility=hidden']):
                opts.append('-fvisibility=hidden')
        elif ct == 'msvc':
            opts.append(
                '/DVERSION_INFO=\\"%s\\"' % self.distribution.get_version()
            )
        for ext in self.extensions:
            ext.extra_compile_args = opts
            ext.extra_link_args = extra_link_args
        build_ext.build_extensions(self)



with open("README.md", "r") as fh:
    long_description = fh.read()

setup(
    name='glove_py',
    version=__version__,
    author='Wang Heng',
    author_email='me@lightgoing.com',
    description='glove Python bindings',
    long_description=long_description,
    long_description_content_type="text/markdown",
    ext_modules=ext_modules,
    url='https://github.com/f20500909/glove_py',
    classifiers=['Programming Language :: Python :: 3', ],
    install_requires=['pybind11>=2.2', "setuptools >= 0.7.0", "numpy"],
    # cmdclass={'build_ext': BuildExt},
    packages=["glove", "glove.pybind", ],
    # package_dir表示一种映射关系,此处表示包的根目录为当前的python文件夹
    package_dir={'': 'python'},
    zip_safe=False,
)
