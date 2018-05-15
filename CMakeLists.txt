project(bip39)
cmake_minimum_required(VERSION 3.4)

set(CMAKE_CXX_STANDARD 14)

include(cmake/FindSSE.cmake)
findsse()

set(BTC_SRC
    src/crypto/common.h
    src/crypto/sha256.h
    src/crypto/sha256.cpp
    src/crypto/hmac_sha256.h
    src/crypto/hmac_sha256.cpp
    src/crypto/hmac_sha512.h
    src/crypto/hmac_sha512.cpp
    src/crypto/sha512.h
    src/crypto/sha512.cpp
    )

if (SSE41_FOUND OR SSE42_FOUND)
	set(BTC_SRC ${BTC_SRC} src/crypto/sha256_sse4.cpp)
else ()
	set(BTC_SRC ${BTC_SRC} src/crypto/sha256.cpp)
endif ()


set(SOURCES
    ${BTC_SRC}
    src/bip39.cpp
    src/bip39.h
    src/data/wordlists/chinese_simplified.cpp
    src/data/wordlists/chinese_traditional.cpp
    src/data/wordlists/english.cpp
    src/data/wordlists/french.cpp
    src/data/wordlists/italian.cpp
    src/data/wordlists/japanese.cpp
    src/data/wordlists/spanish.cpp
    src/wordlist.cpp
    src/wordlist.h
    src/mnemonic.h
    src/mnemonic.cpp
    src/internal.cpp
    src/internal.h
    src/bip39_core.h
    src/config.h

    src/minter/utils.h src/minter/utils.cpp
    src/minter/HDKeyEncoder.cpp src/minter/HDKeyEncoder.h
    src/minter/Bip39Mnemonic.cpp src/minter/Bip39Mnemonic.h
    src/minter/uint256_t.hpp
)

include_directories(libs/trezor-crypto)

set(JNI_SOURCES
    src/java/network_minter_mintercore_bip39_NativeBip39.h
    src/java/network_minter_mintercore_bip39_NativeBip39.cpp
    src/java/network_minter_mintercore_bip39_NativeHDKeyEncoder.h
    src/java/network_minter_mintercore_bip39_NativeHDKeyEncoder.cpp
    )

include_directories(src/)
include_directories(libs/uint256_t)


add_library(${PROJECT_NAME}_core SHARED ${SOURCES})
target_compile_options(${PROJECT_NAME}_core PUBLIC -Wno-shift-count-overflow)
add_subdirectory(libs/trezor-crypto)
target_link_libraries(${PROJECT_NAME}_core trezor_crypto)


if (ANDROID_PLATFORM)
	add_definitions(-D__ANDROID__)
	add_library(${PROJECT_NAME}_jni SHARED ${JNI_SOURCES})
	target_link_libraries(${PROJECT_NAME}_jni ${PROJECT_NAME}_core)
endif ()

if (RUN)
	add_executable(bip39test src/main.cpp)
	target_link_libraries(bip39test ${PROJECT_NAME}_core)
endif ()


