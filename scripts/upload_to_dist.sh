#!/usr/bin/env bash

#使用之前,确保已经更改了版本号
#清空所有文件,以防止上次重复的文件
cd ..
rm -rf dist/*

python3 setup.py sdist

python3 -m twine upload dist/*
