cmake_minimum_required(VERSION 3.4)
project(trezor_crypto)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)


set(SOURCES
    address.cpp
    address.h
    base58.cpp
    base58.h
    bignum.cpp
    bignum.h
    blake256.cpp
    blake256.h
    blake2_common.h
    blake2b.cpp
    blake2b.h
    blake2s.cpp
    blake2s.h
    check_mem.h
    curves.cpp
    curves.h
    ecdsa.cpp
    ecdsa.h
    groestl.cpp
    groestl.h
    groestl_internal.h
    hasher.cpp
    hasher.h
    hmac.cpp
    hmac.h
    memzero.cpp
    memzero.h
    options.h
    pbkdf2.cpp
    pbkdf2.hpp
    rand.cpp
    rand.h
    ripemd160.cpp
    ripemd160.h
    script.cpp
    script.h
    secp256k1.cpp
    secp256k1.h
    sha2.cpp
    sha2.hpp
    sha3.cpp
    sha3.h
    )

add_library(trezor_crypto ${SOURCES})
add_definitions(-DUSE_KECCAK=1 -DUSE_MONERO=1 -DNO_VALGRIND=1 -DUSE_ETHEREUM=1 -DUSE_GRAPHENE=1)


