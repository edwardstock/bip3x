#!/usr/bin/env bash

REPO_TYPE=$1

case "${REPO_TYPE}" in
centos | fedora)
  cat <<REPO >/etc/yum.repos.d/edwardstock.repo
[edwardstock]
name=edwardstock
baseurl=https://edwardstock.jfrog.io/artifactory/rhel/${REPO_TYPE}/\$releasever/\$basearch
enabled=1
gpgcheck=0
gpgkey=https://edwardstock.jfrog.io/artifactory/rhel/${REPO_TYPE}/\$releasever/\$basearch/repodata/repomd.xml.key
repo_gpgcheck=1
REPO
  ;;

debian)
  EXIST_REPO=$(cat /etc/apt/sources.list | grep "edwardstock.jfrog.io" | head -n 1)
  if [ "${EXIST_REPO}" == "" ]; then
    echo "deb https://edwardstock.jfrog.io/artifactory/debian $(lsb_release -c -s) main" | tee -a /etc/apt/sources.list
  fi
  curl -s https://edwardstock.jfrog.io/artifactory/api/gpg/key/public | apt-key add -
  ;;
*)
  echo "Unknown repo type"
  exit 1
  ;;

esac
