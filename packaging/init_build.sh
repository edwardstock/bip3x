#!/usr/bin/env bash

# fetching cmake
CMAKE_MAJOR="3.12"
CMAKE_PATCH="4"
CMAKE_VERS="${CMAKE_MAJOR}.${CMAKE_PATCH}"
if [ ! -f "${PWD}/cmake-${CMAKE_VERS}-Linux-x86_64.sh" ]
then
    wget https://cmake.org/files/v${CMAKE_MAJOR}/cmake-${CMAKE_VERS}-Linux-x86_64.sh
fi

if [ ! -f "/usr/bin/cmake" ]
then
sh cmake-${CMAKE_VERS}-Linux-x86_64.sh --skip-license --prefix=/usr
fi






