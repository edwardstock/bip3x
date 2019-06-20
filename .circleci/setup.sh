#!/usr/bin/env bash

if [ ! -f "/usr/bin/g++" ]
then
    apt-get update
    apt-get install -y python python3 python3-pip gcc g++ gdb git make curl wget default-jdk
fi

if [ ! -d "/tmp/pkgs" ]
then
    mkdir -p /tmp/pkgs
fi

# fetching cmake

if [ ! -f "/tmp/pkgs/cmake.sh" ]
then
    wget -O /tmp/pkgs/cmake.sh https://github.com/Kitware/CMake/releases/download/v3.14.5/cmake-3.14.5-Linux-x86_64.sh
fi

if [ ! -f "/usr/bin/cmake" ]
then
    sh /tmp/pkgs/cmake.sh --skip-license --prefix=/usr
fi

# github uploader
ghr_file="ghr_v0.12.1_linux_amd64"
if [ ! -f "/tmp/pkgs/ghr.tar.gz" ]
then
  wget -O /tmp/pkgs/ghr.tar.gz https://github.com/tcnksm/ghr/releases/download/v0.12.1/${ghr_file}.tar.gz
fi

if [ ! -f "/usr/bin/ghr" ]
then
  tar -xvf /tmp/pkgs/ghr.tar.gz -C /tmp/pkgs
  cp /tmp/pkgs/ghr_*/ghr /usr/bin
fi


