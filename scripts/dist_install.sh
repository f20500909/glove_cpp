#!/usr/bin/env bash


cd ..
rm -rf dist/*
#python3 setup.py sdist

python3 setup.py bdist_wheel
pip3 install dist/*
