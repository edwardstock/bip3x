#!/usr/bin/env bash

function findCmake {
	local res=$(find /usr/local/ -name "cmake" | grep "/bin" | sort | head -1)
	echo "${res}"
}

function checkCmakeVers {
	if [ "$1" == "" ]
	then
		echo "0"
		return
	fi

	local version_major=$($1 --version | grep version | awk '{print $3}' | awk -F '.' '{print $1}')
	if (( version_major < 3 )); then
	    echo "0"
	else
		echo "1"
	fi
}

# trying to use default version first
CMAKE_BIN=$(which cmake | tr '\n' ' ')
CMAKE_VALID=$(checkCmakeVers ${CMAKE_BIN})

if [ "${CMAKE_VALID}" -eq 0 ]
then
	echo "CMake version must be >= 3.0.0. Trying to search in /usr/local"
	CMAKE_BIN=$(findCmake)
	CMAKE_VALID=$(checkCmakeVers ${CMAKE_BIN})
	if [ "${CMAKE_BIN}" == "" ] || [ "${CMAKE_VALID}" -eq 0 ]
	then
		echo "Didn't found correct version of CMake. Exiting..."
		exit 1
	else
		echo "Found correct cmake: ${CMAKE_BIN}"
	fi
fi

mkdir -p build && cd build
${CMAKE_BIN} ../ -DCMAKE_BUILD_TYPE=Release
make