#!/usr/bin/env bash


cd ..
rm -rf dist/*

python3 setup.py sdist

pip3 install dist/*
