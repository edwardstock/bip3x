#!/usr/bin/env bash
set -x

PROJECT="bip39"

#jessie buster
PLATFORMS=(stretch)
PLATFORM_PACKAGE=(deb)

VM_PREFIX="${PROJECT}_build_"
VM_ID_PREFIX="${PROJECT}_"

#deb: jessie wheezy
#rpm: centos6 centos7
#???

buildRootPath="/root/_build"

for index in ${!PLATFORMS[*]}
do

    platform=${PLATFORMS[$index]}
    pkg=${PLATFORM_PACKAGE[$index]}
    outputDir=${PWD}/_build/${platform}
    vm_name="${VM_PREFIX}${platform}"
    vm_id="${VM_ID_PREFIX}${platform}"

    mkdir -p ${outputDir}

    echo " -- Build for ${platform}"
    docker build --rm -t ${PROJECT}_${platform}:${index} -f ${PWD}/packaging/build_${platform}.docker ${PWD}
    containerId=$(docker run --name ${vm_name} -id ${vm_id}:${index})
    $(docker exec ${vm_name} tar -zcvf ${buildRootPath}/libbip39_${platform}.tar.gz ${buildRootPath}/libbip39_core.so ${buildRootPath}/libbip39_go.a ${buildRootPath}/libbip39_jni.so)
    builtPackage="${buildRootPath}/libbip39_${platform}.tar.gz"

    echo -e "\n -- Copying ${builtPackage} to ${PWD}/_build\n"
    mkdir -p ${PWD}/_build/${platform}
    docker cp ${vm_name}:${builtPackage} ${PWD}/_build/${platform}

    echo -e "\n -- Cleanup"
    docker stop ${containerId}

    if [ "$1" != "nodelete" ]
    then
    docker rm ${containerId}
    docker rmi ${vm_id}:${index}
    fi

    echo -e "\n"
done

# debian stretch
