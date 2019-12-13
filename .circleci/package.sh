#!/usr/bin/env bash
rootDir=$(pwd)
mkdir -p output
mkdir -p _build_package && cd _build_package

rm -rf ${rootDir}/output/*
rm -rf _install

cmake .. -DCMAKE_BUILD_TYPE=Debug -DENABLE_CONAN=Off -DENABLE_BIP39_C=On -DENABLE_TEST=Off -DENABLE_BIP39_JNI=Off -DCMAKE_INSTALL_PREFIX=$(pwd)/_install
cmake --build . -- -j4
cmake --build . --target install

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

arch=$(uname -m)
sysname=$(uname)
sysname=$(to_lower ${sysname})
gvers=$(git tag | sort -r | head -n 1)
ghash=$(git rev-parse --short=8 HEAD)
fname_sufix="v${gvers}-${ghash}-${sysname}-${arch}"
fname="libbip39-${fname_sufix}.tar.gz"

cd $(pwd)/_install

tar -zcvf "${fname}" .

mv "${fname}" ${rootDir}/output/
cd ${rootDir}/output

ls -lsa $(pwd)

ghr ${VERS} .










