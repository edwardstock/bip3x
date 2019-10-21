#!/usr/bin/env bash
set -e
set -x
VERS=`git rev-parse --short HEAD`
if [ -f "version" ]
then
    VERS=`cat version | tr -d "\n"`
fi

if [ -f "../version" ]
then
    VERS=`cat ../version | tr -d "\n"`
fi

if [ "${1}" != "" ]
then
    VERS=${1}
fi

sysname=$(uname)

stdlibname="libstdc++"
if [ "${sysname}" == "Darwin" ]
then
  stdlibname="libc++"
fi

## Deploy in latest channel
if [ "${sysname}" == "Linux" ]
then
  CONAN_LOCAL=1 conan create . edwardstock/latest -s compiler.libcxx=${stdlibname}11 -s build_type=Debug --build=missing
  CONAN_LOCAL=1 conan create . edwardstock/latest -s compiler.libcxx=${stdlibname}11 -s build_type=Release --build=missing
fi

CONAN_LOCAL=1 conan create . edwardstock/latest -s compiler.libcxx=${stdlibname} -s build_type=Debug --build=missing
CONAN_LOCAL=1 conan create . edwardstock/latest -s compiler.libcxx=${stdlibname} -s build_type=Release --build=missing
CONAN_LOCAL=1 conan upload bip39/${VERS}@edwardstock/latest --all -r=edwardstock