cmake_minimum_required(VERSION 3.4)
project(bip3x
        VERSION 2.2.1
        DESCRIPTION "Native BIP39 Mnemonic implementation (with JNI and pure C bindings)"
        LANGUAGES CXX)

set(LIB_NAME_MAIN bip39)
set(LIB_NAME_C cbip39)
set(LIB_NAME_JNI bip39_jni)

cmake_policy(SET CMP0087 NEW)
cmake_policy(SET CMP0074 NEW)

include(GNUInstallDirs)
set(CMAKE_CXX_STANDARD 14)

if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY lib)
set(CMAKE_PDB_OUTPUT_DIRECTORY lib)

set(ENABLE_SHARED OFF CACHE BOOL "Build shared core library instead of static")
set(ENABLE_TEST OFF CACHE BOOL "Compile unit google tests")
set(ENABLE_BIP39_JNI ON CACHE BOOL "Enable compiling for android/jre")
set(ENABLE_BIP39_C ON CACHE BOOL "Enable compiling pure C for other language bindings")
set(ENABLE_CONAN ON CACHE BOOL "Enable conan dependencies instead of system")
option(USE_OPENSSL_RANDOM OFF "Use OpenSSL random for mnemonic generation")

if (MINGW AND NOT USE_OPENSSL_RANDOM)
	message(WARNING "You're using MinGW without OpenSSL random generator. Be carefully, it's not secure. Consider to use -DUSE_OPENSSL_RANDOM=On and install OpenSSL >= 1.0.0")
endif ()

if (USE_OPENSSL_RANDOM)
	list(APPEND CONAN_OPTS "with_openssl_rand=True")
endif ()

if (CYGWIN AND ENABLE_CONAN)
	message(FATAL_ERROR "Conan does not support Cygwin binaries")
endif ()

if (WIN32)
	execute_process(COMMAND cmd /C "echo ${CMAKE_PROJECT_VERSION} > ${CMAKE_CURRENT_SOURCE_DIR}/version")
else ()
	execute_process(COMMAND bash -c "echo ${CMAKE_PROJECT_VERSION} > ${CMAKE_CURRENT_SOURCE_DIR}/version")
endif ()

if (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W2 /MP")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /DEBUG /Od")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")
	string(REGEX REPLACE "\\/W3" " " CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
	add_compile_options(/wd4146)
	add_compile_options(/wd4251)
	add_compile_options(/wd4838)
elseif (MINGW)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Wno-unknown-pragmas")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -pedantic")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -g0 -O2")
else ()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-shift-count-overflow -fPIC -Wno-c99-extensions")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -pedantic -Wunused-function")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -g0 -O2")
endif ()


list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/modules)
if (ENABLE_CONAN)
	include(ConanInit)
	include(conan_fallback)
endif ()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_definitions(-DUSE_KECCAK=1 -DUSE_MONERO=1 -DNO_VALGRIND=1 -DUSE_ETHEREUM=1 -DUSE_GRAPHENE=1)

set(BIP39_EXPORTING 1)
if (ENABLE_SHARED)
	set(BIP39_SHARED 1)
endif ()

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
    include/bip3x/crypto/common.h
    include/bip3x/crypto/sha256.h
    include/bip3x/crypto/hmac_sha256.h
    include/bip3x/crypto/hmac_sha512.h
    include/bip3x/crypto/sha512.h
    include/bip3x/bip39.h
    include/bip3x/wordlist.h
    include/bip3x/bip39_core.h
    include/bip3x/config.h
    include/bip3x/uint256_t.hpp
    include/bip3x/PCGRand.hpp
    include/bip3x/utils.h
    include/bip3x/Bip39Mnemonic.h
    include/bip3x/HDKeyEncoder.h
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
    src/bip39.cpp
    src/data/wordlists/chinese_simplified.cpp
    src/data/wordlists/chinese_traditional.cpp
    src/data/wordlists/english.cpp
    src/data/wordlists/french.cpp
    src/data/wordlists/italian.cpp
    src/data/wordlists/japanese.cpp
    src/data/wordlists/spanish.cpp
    src/wordlist.cpp
    src/mnemonic.cpp
    src/minter/utils.cpp
    src/minter/HDKeyEncoder.cpp
    src/minter/Bip39Mnemonic.cpp
    )

set(JNI_SOURCES
    src/java/com_edwardstock_bip3x_NativeBip39.h
    src/java/com_edwardstock_bip3x_NativeBip39.cpp
    src/java/com_edwardstock_bip3x_NativeHDKeyEncoder.h
    src/java/com_edwardstock_bip3x_NativeHDKeyEncoder.cpp
    src/java/nobject.h
    src/java/nobject.cpp
    )

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cfg/bip39_core.h.in
	${CMAKE_CURRENT_SOURCE_DIR}/include/bip3x/bip39_core.h
)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cfg/cbip39_config.h.in
	${CMAKE_CURRENT_SOURCE_DIR}/src/bindings/cbip39_config.h
)

set(INSTALL_TARGETS ${LIB_NAME_MAIN})

if (ENABLE_SHARED)
	message(STATUS "Build bip39 shared libs")
	add_library(${LIB_NAME_MAIN} SHARED ${SOURCES})
	set_property(TARGET ${LIB_NAME_MAIN} PROPERTY SOVERSION ${PROJECT_VERSION_MAJOR})
else ()
	message(STATUS "Build bip39 static libs")
	add_library(${LIB_NAME_MAIN} STATIC ${SOURCES})
endif ()

if (ENABLE_CONAN)
	add_conan_remote(edwardstock https://edwardstock.jfrog.io/artifactory/api/conan/conan)
	conan_init()
else ()
	# toolbox
	find_package(Toolbox 3.2.2 REQUIRED)
	set_target_properties(toolbox::toolbox PROPERTIES IMPORTED_GLOBAL TRUE)
	add_library(CONAN_PKG::toolbox ALIAS toolbox::toolbox)

endif ()

target_link_libraries(${LIB_NAME_MAIN} CONAN_PKG::toolbox)
if (USE_OPENSSL_RANDOM)
	if (ENABLE_CONAN)
		target_link_libraries(${LIB_NAME_MAIN} CONAN_PKG::openssl)
	else ()
		find_package(OpenSSL 1.0.0 REQUIRED)
		target_link_libraries(${LIB_NAME_MAIN} OpenSSL::Crypto)
	endif ()
endif ()

if (MSVC)
	set_property(TARGET ${LIB_NAME_MAIN} APPEND PROPERTY LINK_FLAGS /version:${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR})
endif ()

set_target_properties(${LIB_NAME_MAIN} PROPERTIES LANGUAGE CXX)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs)

# Pure C
if (ENABLE_BIP39_C)
	set(BIP39_C_HEADERS
	    src/bindings/cbip39_config.h
	    src/bindings/cbip39.h
	    src/bindings/cbip39_hdkey_encoder.h
	    )

	set(BIP39_C_SOURCES
	    ${SOURCES}
	    ${BIP39_C_HEADERS}
	    src/bindings/cbip39.cpp
	    src/bindings/cbip39.cpp
	    src/bindings/cbip39_hdkey_encoder.cpp
	    )

	if (ENABLE_SHARED)
		add_library(${LIB_NAME_C} SHARED ${BIP39_C_SOURCES})
		set_property(TARGET ${LIB_NAME_C} PROPERTY SOVERSION ${PROJECT_VERSION_MAJOR})
	else ()
		add_library(${LIB_NAME_C} STATIC ${BIP39_C_SOURCES})
	endif ()
	# link toolbox again, as we don't link main library, only using sources directly
	target_link_libraries(${LIB_NAME_C} CONAN_PKG::toolbox)

	if (USE_OPENSSL_RANDOM)
		if (ENABLE_CONAN)
			target_link_libraries(${LIB_NAME_C} CONAN_PKG::openssl)
		else ()
			target_link_libraries(${LIB_NAME_C} OpenSSL::Crypto)
		endif ()
	endif ()

	set_target_properties(${LIB_NAME_C} PROPERTIES
	                      PUBLIC_HEADER "${BIP39_C_HEADERS}"
	                      LANGUAGE C99)


	target_include_directories(${LIB_NAME_C} PUBLIC
	                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/bindings>
	                           $<INSTALL_INTERFACE:src/bindings>
	                           )
	target_include_directories(${LIB_NAME_C} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

	list(APPEND INSTALL_TARGETS ${LIB_NAME_C})
endif ()

# Android/JAVA
if (ENABLE_BIP39_JNI)
	if (ANDROID_PLATFORM)
		add_definitions(-D__ANDROID__)
		if (ENABLE_SHARED)
			add_library(${LIB_NAME_JNI} SHARED ${JNI_SOURCES})
		else ()
			add_library(${LIB_NAME_JNI} STATIC ${JNI_SOURCES})
		endif ()
		target_link_libraries(${LIB_NAME_JNI} ${LIB_NAME_MAIN})
	else ()
		find_package(JNI)
		if (JNI_FOUND)
			if (ENABLE_SHARED)
				add_library(${LIB_NAME_JNI} SHARED ${JNI_SOURCES})
				set_property(TARGET ${LIB_NAME_JNI} PROPERTY SOVERSION ${PROJECT_VERSION_MAJOR})
			else ()
				add_library(${LIB_NAME_JNI} STATIC ${JNI_SOURCES})
			endif ()
			target_link_libraries(${LIB_NAME_JNI} ${LIB_NAME_MAIN})
			target_link_libraries(${LIB_NAME_JNI} CONAN_PKG::toolbox)
			target_link_libraries(${LIB_NAME_JNI} ${JAVA_JVM_LIBRARY})
			target_include_directories(${LIB_NAME_JNI} PRIVATE ${JNI_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/libs)

			list(APPEND INSTALL_TARGETS ${LIB_NAME_JNI})
		else ()
			message(STATUS "JNI not found")
		endif ()
	endif ()
endif ()

if (${RUN})
	add_executable(bip39dev src/main.cpp)
	target_link_libraries(bip39dev ${LIB_NAME_MAIN})
	#	target_compile_options(bip39dev PUBLIC -fsanitize=address -m64 -pipe -ffunction-sections -fdata-sections  -O0 -g -fno-omit-frame-pointer)
endif ()

if (ENABLE_PVS)
	include(cmake/PVS-Studio.cmake)

	pvs_studio_add_target(TARGET ${LIB_NAME_MAIN}.analyze ALL
	                      OUTPUT FORMAT errorfile
	                      CONFIG ${CMAKE_CURRENT_SOURCE_DIR}/pvs.cfg
	                      ANALYZE ${LIB_NAME_MAIN} ${LIB_NAME_C}
	                      LOG target_main.err)
endif ()


if (ENABLE_TEST)
	if (NOT ENABLE_CONAN)
		if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/libs/gtest/")
			message(FATAL_ERROR "Unable to build test target as gtest not found in ${CMAKE_CURRENT_SOURCE_DIR}/libs. Please try to update git submodules.")
		endif ()
		add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/gtest)
		add_library(CONAN_PKG::gtest ALIAS gtest)
	endif ()

	set(gtest_force_shared_crt ${ENABLE_SHARED} CACHE BOOL "" FORCE)

	if (MSVC)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++14")
	elseif (APPLE)
		add_definitions(-DGTEST_USE_OWN_TR1_TUPLE)
		add_definitions(-D__GLIBCXX__)
	endif ()

	include(GoogleTest)

	# main test
	set(PROJECT_TEST_NAME "bip39_test")
	enable_testing()

	set(TEST_SOURCES
	    tests/main.cpp
	    tests/minter_cxx_test.cpp
	    )

	if (ENABLE_BIP39_C)
		set(TEST_SOURCES ${TEST_SOURCES}
		    tests/minter_cc_test.cpp)
	endif ()

	add_executable(${PROJECT_TEST_NAME} ${TEST_SOURCES})
	target_link_libraries(${PROJECT_TEST_NAME} ${LIB_NAME_MAIN})

	if (ENABLE_BIP39_C)
		target_link_libraries(${PROJECT_TEST_NAME} ${LIB_NAME_C})
		target_include_directories(${PROJECT_TEST_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/bindings)
	endif ()

	target_link_libraries(${PROJECT_TEST_NAME} CONAN_PKG::gtest)
	target_include_directories(${PROJECT_TEST_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
	target_include_directories(${PROJECT_TEST_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs)
	gtest_add_tests(TARGET ${PROJECT_TEST_NAME})

	if (ENABLE_BIP39_SANITIZE)
		target_link_libraries(${PROJECT_TEST_NAME} asan)
		target_compile_options(${PROJECT_TEST_NAME} PUBLIC -fsanitize=address -m64 -pipe -ffunction-sections -fdata-sections -O0 -g -fno-omit-frame-pointer)
	endif ()

endif ()

include(package)


