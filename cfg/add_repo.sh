#!/usr/bin/env bash

REPO_TYPE=$1

case "${REPO_TYPE}" in
centos | fedora)
  cat <<REPO >/etc/yum.repos.d/bintray-edwardstock.repo
[bintray-edwardstock]
name=bintray-edwardstock
baseurl=https://dl.bintray.com/edwardstock/rh/${REPO_TYPE}/\$releasever/\$basearch
gpgcheck=0
repo_gpgcheck=0
enabled=1
REPO

  curl -s https://bintray.com/user/downloadSubjectPublicKey?username=bintray | gpg --import
  ;;

debian)
  EXIST_REPO=$(cat /etc/apt/sources.list | grep edwardstock/debian | head -n 1)
  if [ "${EXIST_REPO}" == "" ]; then
    echo "deb https://dl.bintray.com/edwardstock/debian $(lsb_release -c -s) main" | tee -a /etc/apt/sources.list
  fi
  curl -s https://bintray.com/user/downloadSubjectPublicKey?username=bintray | apt-key add -
  ;;
*)
  echo "Unknown repo type"
  exit 1
  ;;

esac
