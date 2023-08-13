if (CMAKE_CONAN_BUILD)
	cmake_minimum_required(VERSION 3.24)
else ()
	cmake_minimum_required(VERSION 3.19)
endif ()
project(bip3x
        VERSION 3.0.0
        DESCRIPTION "Native BIP39 Mnemonic implementation (with JNI and pure C bindings)"
        LANGUAGES CXX)

set(LIB_NAME_MAIN bip3x)
set(LIB_NAME_C c${PROJECT_NAME})
set(LIB_NAME_JNI ${PROJECT_NAME}_jni)

cmake_policy(SET CMP0087 NEW)
cmake_policy(SET CMP0074 NEW)
cmake_policy(SET CMP0077 NEW)

include(GNUInstallDirs)
set(CMAKE_CXX_STANDARD 14)

option(bip3x_BUILD_TESTS "Build tests" OFF)
option(bip3x_BUILD_EXAMPLE "Build example" OFF)
option(bip3x_BUILD_SHARED_LIBS "Build shared library instead of static" OFF)
option(bip3x_BUILD_JNI_BINDINGS "Additionally build JNI shared library" OFF)
option(bip3x_BUILD_C_BINDINGS "Additionally build C library" OFF)
option(bip3x_USE_OPENSSL_RANDOM "Use openssl random number generator" OFF)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

add_definitions(-DBIP3X_EXPORTS)
if (NOT bip3x_BUILD_SHARED_LIBS)
	add_definitions(-DBIP3X_BUILT_AS_STATIC)
endif ()

if (NOT WIN32)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY lib)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY lib)
endif ()

if (MINGW AND NOT bip3x_USE_OPENSSL_RANDOM)
	message(WARNING "You're using MinGW without OpenSSL random generator. Be carefully, it's not secure. Consider to use -Dbip3x_USE_OPENSSL_RANDOM=On and install OpenSSL >= 1.0.0")
endif ()

if (CMAKE_CONAN_BUILD)
	if (bip3x_USE_OPENSSL_RANDOM)
		conan_set_option("with_openssl_rand=True")
	endif ()
	if (bip3x_BUILD_TESTS)
		conan_set_option("with_tests=True")
	endif ()
endif ()

if (CYGWIN AND CMAKE_CONAN_BUILD)
	message(FATAL_ERROR "Conan does not support Cygwin binaries")
endif ()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cfg/version.info.in ${CMAKE_CURRENT_SOURCE_DIR}/version.info @ONLY NEWLINE_STYLE UNIX)

# compiler flags
include(compiler_flags)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_definitions(-DUSE_KECCAK=1 -DUSE_MONERO=1 -DNO_VALGRIND=1 -DUSE_ETHEREUM=1 -DUSE_GRAPHENE=1)

set(BIP39_EXPORTING 1)
if (ENABLE_SHARED)
	set(BIP39_SHARED 1)
endif ()

configure_file(
	${CMAKE_CURRENT_LIST_DIR}/cfg/bip3x_config.in.h
	${CMAKE_CURRENT_LIST_DIR}/include/bip3x/bip3x_config.h
	NEWLINE_STYLE UNIX
)

configure_file(
	${CMAKE_CURRENT_LIST_DIR}/cfg/cbip39_config.in.h
	${CMAKE_CURRENT_LIST_DIR}/include/cbip3x/cbip3x_config.h
	NEWLINE_STYLE UNIX
)

## SOURCES BEGIN
set(BTC_SRC)
set(BTC_HEADERS)

if (WIN32)
	set(BTC_HEADERS
	    include/bip3x/crypto/win_endian.h
	)
endif ()

if (SSE41_FOUND OR SSE42_FOUND)
	set(BTC_SRC ${BTC_SRC} src/crypto/sha256_sse4.cpp)
else ()
	set(BTC_SRC ${BTC_SRC} src/crypto/sha256.cpp)
endif ()

set(TREZOR_HEADERS
    include/bip3x/crypto/address.h
    include/bip3x/crypto/base58.h
    include/bip3x/crypto/bignum.h
    include/bip3x/crypto/blake256.h
    include/bip3x/crypto/check_mem.h
    include/bip3x/crypto/ecdsa.h
    include/bip3x/crypto/hasher.h
    include/bip3x/crypto/hmac.h
    include/bip3x/crypto/memzero.h
    include/bip3x/crypto/options.h
    include/bip3x/crypto/pbkdf2.hpp
    include/bip3x/crypto/rfc6979.h
    include/bip3x/crypto/rand.h
    include/bip3x/crypto/ripemd160.h
    include/bip3x/crypto/secp256k1.h
    include/bip3x/crypto/sha2.hpp
    include/bip3x/crypto/sha3.h
    include/bip3x/crypto/packed_attr.h
)

set(TREZOR_SOURCES
    ${TREZOR_HEADERS}
    src/trezor-crypto/address.cpp
    src/trezor-crypto/base58.cpp
    src/trezor-crypto/bignum.cpp
    src/trezor-crypto/blake256.cpp
    src/trezor-crypto/ecdsa.cpp
    src/trezor-crypto/hasher.cpp
    src/trezor-crypto/hmac.cpp
    src/trezor-crypto/memzero.cpp
    src/trezor-crypto/pbkdf2.cpp
    src/trezor-crypto/rand.cpp
    src/trezor-crypto/ripemd160.cpp
    src/trezor-crypto/secp256k1.cpp
    src/trezor-crypto/sha2.cpp
    src/trezor-crypto/sha3.cpp
)

set(HEADERS
    ${BTC_HEADERS}
    include/bip3x/bip3x_config.h
    include/bip3x/crypto/common.h
    include/bip3x/crypto/sha256.h
    include/bip3x/crypto/hmac_sha256.h
    include/bip3x/crypto/hmac_sha512.h
    include/bip3x/crypto/sha512.h
    include/bip3x/details/uint256_t.hpp
    include/bip3x/details/utils.h
    include/bip3x/details/PCGRand.hpp
    include/bip3x/bip3x_mnemonic.h
    include/bip3x/bip3x_hdkey_encoder.h
    include/bip3x/bip3x_crypto.h
)

set(SOURCES
    ${HEADERS}
    ${BTC_SRC}
    ${TREZOR_SOURCES}
    src/crypto/sha256.cpp
    src/crypto/sha256_sse4.cpp
    src/crypto/hmac_sha256.cpp
    src/crypto/hmac_sha512.cpp
    src/crypto/sha512.cpp

    src/bip3x/utils.cpp
    src/bip3x/bip3x_hdkey_encoder.cpp
    src/bip3x/bip3x_mnemonic.cpp

    src/details/bip39.h
    src/details/bip39.cpp
    src/details/config.h
    src/details/mnemonic.h
    src/details/mnemonic.cpp
    src/details/wordlist.h
    src/details/wordlist.cpp
    src/details/wordlists/chinese_simplified.cpp
    src/details/wordlists/chinese_traditional.cpp
    src/details/wordlists/english.cpp
    src/details/wordlists/french.cpp
    src/details/wordlists/italian.cpp
    src/details/wordlists/japanese.cpp
    src/details/wordlists/spanish.cpp
    src/bip3x/bip3x_crypto.cpp
)
## SOURCES END


# Add target
if (bip3x_BUILD_SHARED_LIBS)
	message(STATUS "Build shared libs")
	add_library(${LIB_NAME_MAIN} SHARED ${SOURCES})
else ()
	message(STATUS "Build static libs")
	add_library(${LIB_NAME_MAIN} STATIC ${SOURCES})
endif ()
target_include_directories(${LIB_NAME_MAIN} PUBLIC
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                           $<INSTALL_INTERFACE:include>
)
set_target_properties(${LIB_NAME_MAIN} PROPERTIES LANGUAGE CXX)

if (CMAKE_CONAN_BUILD)
	find_package(toolbox CONFIG REQUIRED)
else ()
	include(FetchContent)
	# toolbox
	fetchcontent_declare(
		toolbox
		GIT_REPOSITORY https://github.com/edwardstock/toolbox.git
		GIT_TAG 3.4.0
	)
	set(toolbox_BUILD_TESTS OFF)
	set(toolbox_BUILD_SHARED_LIBS OFF)

	fetchcontent_makeavailable(toolbox)
	add_library(toolbox::toolbox ALIAS toolbox)
endif ()

target_link_libraries(${LIB_NAME_MAIN} toolbox::toolbox)

if (bip3x_USE_OPENSSL_RANDOM)
	if (CMAKE_CONAN_BUILD)
		find_package(OpenSSL CONFIG REQUIRED)
		target_link_libraries(${LIB_NAME_MAIN} openssl::openssl)
	else ()
		find_package(OpenSSL 1.0.0 REQUIRED)
		target_link_libraries(${LIB_NAME_MAIN} OpenSSL::Crypto)
	endif ()
endif ()


# Pure C
if (bip3x_BUILD_C_BINDINGS)
	include(target_c)
endif ()

# Android/JAVA
if (bip3x_BUILD_JNI_BINDINGS)
	include(target_jni)
endif ()

if (bip3x_BUILD_EXAMPLE)
	add_executable(bip3x_example src/example.cpp)
	target_link_libraries(bip3x_example ${LIB_NAME_MAIN})
endif ()


if (bip3x_BUILD_TESTS)
	include(target_test)
endif ()

include(package)
