#!/usr/bin/env bash

set -e

mkdir -p _build && cd _build
rm -rf ./*
cmake .. -DCMAKE_BUILD_TYPE=Debug -DENABLE_TEST=On
make -j4
make test
