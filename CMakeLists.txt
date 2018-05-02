project(bip39)
cmake_minimum_required(VERSION 3.4)

set(CMAKE_CXX_STANDARD 14)


find_package(JNI REQUIRED)
include_directories(${JNI_INCLUDE_DIRS})

set(SOURCES
    src/bip39.c
    src/bip39.h
    src/data/wordlists/chinese_simplified.c
    src/data/wordlists/chinese_traditional.c
    src/data/wordlists/english.c
    src/data/wordlists/french.c
    src/data/wordlists/italian.c
    src/data/wordlists/japanese.c
    src/data/wordlists/spanish.c
    src/hmac.c
    src/hmac.h
    src/hmac.inl
    src/wordlist.c
    src/wordlist.h
    src/mnemonic.h
    src/mnemonic.c
    src/internal.c
    src/internal.h
    src/wally_crypto.h
    src/ccan/ccan/build_assert/build_assert.h
    src/ccan/ccan/crypto/ripemd160/ripemd160.h
    src/ccan/ccan/crypto/sha256/sha256.h
    src/ccan/ccan/crypto/sha512/sha512.h
    src/bit39_core.h
    src/config.h)

set(JNI_SOURCES
    src/java/NativeBip39.h
    src/java/NativeBip39.cpp src/java/NativeBip39.h)

include_directories(src/)


add_library(${PROJECT_NAME}_core STATIC ${SOURCES})
add_library(${PROJECT_NAME}_jni SHARED ${JNI_SOURCES})
target_link_libraries(${PROJECT_NAME}_jni ${PROJECT_NAME}_core)
target_link_libraries(${PROJECT_NAME}_jni ${JNI_LIBRARIES})


