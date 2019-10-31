#include<iostream>
#include<vector>
#include<string>
#include<memory>
#include<pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <Python.h>
#include "../glove.h"

namespace py = pybind11;
using namespace std;




PYBIND11_MODULE(glove_pybind, m) {
    py::class_<Glove>(m, "glove")
            .def(py::init<>())
            .def_readwrite("input_file", &Glove::input_file)
            .def_readwrite("vocab_size", &Glove::vocab_size)
            .def_readwrite("max_size", &Glove::max_size)
            .def_readwrite("min_count", &Glove::min_count)
            .def_readwrite("window", &Glove::window)
            .def_readwrite("epoch", &Glove::epoch)
            .def_readwrite("threads", &Glove::threads)
            .def_readwrite("memory_limit", &Glove::memory_limit)
            .def_readwrite("lr", &Glove::lr)


//            .def(py::init<std::string, std::string, std::string, std::string, std::string, std::string,
//                    unsigned long long, unsigned long long, unsigned long, unsigned long, unsigned long,
//                    unsigned long, int,  double, bool, bool>())


            .def("to_txt", [](Glove &m) { m.to_txt(); })

            .def("run", [](Glove &m, string input_file) { m.train(input_file); })

            .def("most_similary", [](Glove &m, string word, int num) {
                return m.most_similary(word, num);
            })

            .def("load", [](Glove &m,  std::string wordvec_file, std::string meta_filel) {
                m.load( wordvec_file, meta_filel);
            })

            ;
}



