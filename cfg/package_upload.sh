#!/usr/bin/env bash

set -e

TYPE=""
if [ "${DRY_RUN}" != "" ]; then
  DRY_RUN="1"
else
  DRY_RUN=""
fi
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
    curl -Lso /tmp/ghr.tar.gz "https://github.com/tcnksm/ghr/releases/download/v0.12.2/ghr_v0.12.2_linux_amd64.tar.gz"
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

elif [ "${TYPE}" == "bintray" ]; then
  JFROG_BIN=${BUILD_ROOT}/jfrog

  if [ ! -f "/usr/bin/jfrog" ] && [ ! -f "/usr/local/bin/jfrog" ]; then
    if [ ! -f "/tmp/jfrog_cli" ]; then
      curl -Lso /tmp/jfrog_cli https://jfrog.bintray.com/jfrog-cli-go/1.39.5/jfrog-cli-linux-amd64/jfrog
    fi
    if [ ! -f "${JFROG_BIN}" ]; then
      cp /tmp/jfrog_cli ${JFROG_BIN}
      chmod +x ${JFROG_BIN}
    fi
  else
    JFROG_BIN=$(which jfrog)
  fi

  #1. configure
  ${JFROG_BIN} bt c --user $BINTRAY_USER --key $BINTRAY_API_KEY --licenses MIT

  #2. create package if not exists
  ${JFROG_BIN} bt pc --vcs-url @CPACK_PACKAGE_VCS_URL@ ${BINTRAY_USER}/@REPO_NAME@/@PROJECT_NAME@ >/dev/null 2>&1 || true

  #3. upload
  if [ "${DRY_RUN}" == "1" ]; then
    echo ${JFROG_BIN} bt upload --publish --override @JFROG_OPTIONS@ "${BUILD_ROOT}/@UPLOAD_FILE_NAME@" "${BINTRAY_USER}/@REPO_NAME@/@PROJECT_NAME@/@PROJECT_VERSION@" "@TARGET_PATH@"
    exit 0
  fi

  ${JFROG_BIN} bt upload --publish --override @JFROG_OPTIONS@ "${BUILD_ROOT}/@UPLOAD_FILE_NAME@" "${BINTRAY_USER}/@REPO_NAME@/@PROJECT_NAME@/@PROJECT_VERSION@" "@TARGET_PATH@"

fi
