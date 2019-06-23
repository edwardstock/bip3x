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

## Deploy in latest channel
CONAN_LOCAL=1 conan create . edwardstock/latest
CONAN_LOCAL=1 conan create . edwardstock/latest -s compiler.libcxx=libstdc++11
CONAN_LOCAL=1 conan upload bip39/${VERS}@edwardstock/latest --all -r=edwardstock