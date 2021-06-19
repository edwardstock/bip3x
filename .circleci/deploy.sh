#!/usr/bin/env bash
set -e

###### DEFINE PROJECT CONFIG HERE ######
CONAN_PROJECT_NAME="bip39"
CONAN_REPO="edwardstock"
CONAN_REPO_TAG="latest"
# only boolean options supported
BUILD_OPTIONS=(shared enableJNI enableC with_openssl_rand)

###### END PROJECT CONFIG ##############

if [ -f "version" ]; then
  VERS=$(cat version | tr -d "\n")
fi

if [ -f "../version" ]; then
  VERS=$(cat ../version | tr -d "\n")
fi

if [ "${1}" != "" ]; then
  VERS=${1}
fi

sysname=$(uname)

std_lib_names=(libstdc++ libstdc++11)
if [ "${sysname}" == "Darwin" ]; then
  std_lib_names=(libc++)
fi

BOOL_VALS=(True False)
BUILD_OPTS_PRE=()
TARGET_OPTS=()

opts_cnt=${#BUILD_OPTIONS[@]}
bool_cnt=2

# count = 2^n
# 2 is a vals set: {True; False}
# n is options length
res_cnt=$((bool_cnt ** opts_cnt))
tmp_cnt=$((bool_cnt * opts_cnt))

# create temporary list of flat variants
for ((bool_idx = 0, n = 0; bool_idx < bool_cnt; bool_idx++)); do
  for ((opt_idx = 0; opt_idx < opts_cnt; opt_idx++)); do
    BUILD_OPTS_PRE[$n]="-o ${BUILD_OPTIONS[$opt_idx]}=${BOOL_VALS[$bool_idx]}"
    n=$((n + 1))
  done
done

# create target options list
for ((i = 0; i < res_cnt; i++)); do
  for ((n = 0; n < opts_cnt; n++)); do
    next_index=$(((i + n) % tmp_cnt))
    TARGET_OPTS[$i]="${TARGET_OPTS[$i]} ${BUILD_OPTS_PRE[$next_index]}"
  done
done

set -x

# iterate over each options cartesian product
for opt_idx in ${!TARGET_OPTS[*]}; do
  echo "Build with opt:${TARGET_OPTS[$opt_idx]}"

  # iterate over available c++ stdlib
  for stdlib_idx in ${!std_lib_names[*]}; do
    CONAN_LOCAL=1 conan create . ${CONAN_REPO}/${CONAN_REPO_TAG} -s compiler.libcxx="${std_lib_names[$stdlib_idx]}" -s build_type=Debug --build=missing ${TARGET_OPTS[$opt_idx]}
    CONAN_LOCAL=1 conan create . ${CONAN_REPO}/${CONAN_REPO_TAG} -s compiler.libcxx="${std_lib_names[$stdlib_idx]}" -s build_type=Release --build=missing ${TARGET_OPTS[$opt_idx]}
  done
done

if [ "${NOUPLOAD}" != "1" ]; then
  CONAN_LOCAL=1 conan upload ${CONAN_PROJECT_NAME}/${VERS}@${CONAN_REPO}/${CONAN_REPO_TAG} --all -r=${CONAN_REPO}
fi
