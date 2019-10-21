#!/usr/bin/env bash

set -e

mkdir -p _build && cd _build
cmake .. -DCMAKE_BUILD_TYPE=Debug -DENABLE_BIP39_TESTS=On
make -j4
make test