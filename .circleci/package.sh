#!/usr/bin/env bash
mkdir output
mkdir -p _build_package && cd _build_package


cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=$(pwd)/_install
cmake --build . --target minter_tx_merge -- -j4
cmake -DCOMPONENT=minter-sdk -P cmake_install.cmake

function to_lower() {
  local outRes=$(echo ${1} | awk '{ for ( i=1; i <= NF; i++) {   sub(".", substr(tolower($i),1,1) , $i)  } print }')
  echo "${outRes}"
}

tar_bin=$(which tar | tr -d "\n")
if [ "${tar_bin}" == "" ]
then
  echo "Tar executable not found!"
  exit 1
fi

arch=$(uname -m)
sysname=$(uname)
sysname=$(to_lower ${sysname})
gvers=$(git tag)
ghash=$(git rev-parse --short=8 HEAD)
fname_sufix="${gvers}_${ghash}_${sysname}_${arch}"
fname="libminter_tx_${fname_sufix}.tar.gz"

cd $(pwd)/_install

tar -zcvf ${fname} .

mv ${fname} ../../output
cd ../../output










