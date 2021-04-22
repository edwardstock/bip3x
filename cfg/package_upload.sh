#!/usr/bin/env bash

set -e

TYPE=""
DRY_RUN=""
BUILD_ROOT="@CMAKE_BINARY_DIR@"

function usage() {
  echo ""
  echo ""
  echo "./package_upload.sh"
  echo -e "    -h, --help       |  This help"
  echo -e "    -t, --type       |  Select upload type: github or bintray"
  echo -e "    -b, --buildroot  |  Path where to build project. Default: ${BUILD_ROOT}"
  echo -e "    --dry-run        |  Only echo commands without real uploading"
  echo ""
}

PARAMS=""
while (("$#")); do
  case "$1" in
  -h | --help)
    usage
    exit 0
    ;;
  --dry-run)
    DRY_RUN=1
    shift
    ;;
  -t | --type)
    if [ -n "$2" ] && [ ${2:0:1} != "-" ]; then
      TYPE=$2
      shift 2
    else
      echo "Error: Argument for $1 is missing" >&2
      exit 1
    fi
    ;;
  -b | --buildroot)
    if [ -n "$2" ] && [ ${2:0:1} != "-" ]; then
      BUILD_ROOT=$2
      shift 2
    else
      echo "Error: Argument for $1 is missing" >&2
      exit 1
    fi
    ;;
  -* | --*=) # unsupported flags
    echo "Error: Unsupported flag $1" >&2
    exit 1
    ;;
  *) # preserve positional arguments
    PARAMS="$PARAMS $1"
    shift
    ;;
  esac
done

## CHECK upload type
if [ "${TYPE}" == "" ]; then
  echo "Select type: github or bintray"
  exit 1
fi

## CHECK upload file exists
if [ ! -f "${BUILD_ROOT}/@UPLOAD_FILE_NAME@" ]; then
  echo "File ${BUILD_ROOT}/@UPLOAD_FILE_NAME@ does not exists"
  exit 1
fi

## UPLOAD
if [ "${TYPE}" == "github" ]; then
  # github upload

  if [ ! -f "/tmp/ghr.tar.gz" ]; then
    curl -Lso /tmp/ghr.tar.gz "https://github.com/tcnksm/ghr/releases/download/v0.13.0/ghr_v0.13.0_linux_amd64.tar.gz"
  fi

  GHR_BIN=${BUILD_ROOT}/ghr

  if [ ! -f "/usr/bin/ghr" ] && [ ! -f "/usr/local/bin/ghr" ]; then
    if [ ! -f "${BUILD_ROOT}/ghr" ]; then
      tar -xvf /tmp/ghr.tar.gz -C /tmp
      cp /tmp/ghr_*/ghr ${BUILD_ROOT}/ghr
      chmod +x ${GHR_BIN}
    fi
  else
    GHR_BIN=$(which ghr)
  fi

  if [ "${DRY_RUN}" == "1" ]; then
    echo "${GHR_BIN} -replace @PROJECT_VERSION@ ${BUILD_ROOT}/@UPLOAD_FILE_NAME@"
    exit 0
  fi

  ${GHR_BIN} -replace @PROJECT_VERSION@ ${BUILD_ROOT}/@UPLOAD_FILE_NAME@

elif [ "${TYPE}" == "bintray" ] || [ "${TYPE}" == "artifactory" ]; then
  JFROG_BIN=${BUILD_ROOT}/jfrog

  if [ ! -f "/usr/bin/jfrog" ] && [ ! -f "/usr/local/bin/jfrog" ]; then
    if [ ! -f "/tmp/jfrog_cli" ]; then
      curl -fL https://getcli.jfrog.io | sh
      cp jfrog ${JFROG_BIN}
      chmod +x ${JFROG_BIN}
    fi
  else
    JFROG_BIN=$(which jfrog)
  fi

  # upload
  if [ "${DRY_RUN}" == "1" ]; then
    echo JFROG_CLI_OFFER_CONFIG=false ${JFROG_BIN} rt upload @JFROG_OPTIONS@ "${BUILD_ROOT}/@UPLOAD_FILE_NAME@" "@TARGET_PATH@"
    exit 0
  fi

  JFROG_CLI_OFFER_CONFIG=false ${JFROG_BIN} rt upload @JFROG_OPTIONS@ "${BUILD_ROOT}/@UPLOAD_FILE_NAME@" "@TARGET_PATH@"

fi
