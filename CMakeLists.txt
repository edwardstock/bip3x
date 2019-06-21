cmake_minimum_required(VERSION 3.5)
project(bip39
        VERSION 1.0.0
        DESCRIPTION "Native BIP39 Mnemonic implementation (with JNI bindings)"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

option(ENABLE_SHARED "Build shared core library instead of static" OFF)
option(WITH_BIP39_TEST "Compile unit tests" OFF)
option(ENABLE_JNI "Enable compiling for android/jre" ON)
option(ENABLE_GO "Enable compiling pure C for golang bindings" ON)
option(ENABLE_CONAN_BUILD "Enable conan dependencies" ON)

execute_process(
	COMMAND bash -c "echo ${CMAKE_PROJECT_VERSION} > ${CMAKE_CURRENT_SOURCE_DIR}/version"
)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(ConanInit)

if(ENABLE_CONAN_BUILD)
add_conan_remote(bincrafters https://api.bintray.com/conan/bincrafters/public-conan)
add_conan_remote(edwardstock https://api.bintray.com/conan/edwardstock/conan-public)
conan_init()
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -pedantic")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -g0 -O2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c99-extensions -Wno-unused-function -Wno-unused-const-variable")



include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/configure.cmake)

add_definitions(-DUSE_KECCAK=1 -DUSE_MONERO=1 -DNO_VALGRIND=1 -DUSE_ETHEREUM=1 -DUSE_GRAPHENE=1)

set(BTC_SRC)
set(BTC_HEADERS)

if (WIN32)
	set(BTC_HEADERS
	    include/minter/bip39/crypto/win_endian.h
	    )
endif ()

if (APPLE)
	set(CMAKE_MACOSX_RPATH OFF)
endif (APPLE)

if (SSE41_FOUND OR SSE42_FOUND)
	set(BTC_SRC ${BTC_SRC} src/crypto/sha256_sse4.cpp)
else ()
	set(BTC_SRC ${BTC_SRC} src/crypto/sha256.cpp)
endif ()

set(TREZOR_HEADERS
    include/minter/crypto/address.h
    include/minter/crypto/base58.h
    include/minter/crypto/bignum.h
    include/minter/crypto/blake256.h
    include/minter/crypto/blake2_common.h
    include/minter/crypto/blake2b.h
    include/minter/crypto/blake2s.h
    include/minter/crypto/check_mem.h
    include/minter/crypto/curves.h
    include/minter/crypto/ecdsa.h
    include/minter/crypto/groestl.h
    include/minter/crypto/groestl_internal.h
    include/minter/crypto/hasher.h
    include/minter/crypto/hmac.h
    include/minter/crypto/memzero.h
    include/minter/crypto/options.h
    include/minter/crypto/pbkdf2.hpp
    include/minter/crypto/rfc6979.h
    include/minter/crypto/rand.h
    include/minter/crypto/ripemd160.h
    include/minter/crypto/script.h
    include/minter/crypto/secp256k1.h
    include/minter/crypto/sha2.hpp
    include/minter/crypto/sha3.h
    )

set(TREZOR_SOURCES
    ${TREZOR_HEADERS}
    src/trezor-crypto/address.cpp
    src/trezor-crypto/base58.cpp
    src/trezor-crypto/bignum.cpp
    src/trezor-crypto/blake256.cpp
    src/trezor-crypto/blake2b.cpp
    src/trezor-crypto/blake2s.cpp
    src/trezor-crypto/curves.cpp
    src/trezor-crypto/ecdsa.cpp
    src/trezor-crypto/groestl.cpp
    src/trezor-crypto/hasher.cpp
    src/trezor-crypto/hmac.cpp
    src/trezor-crypto/memzero.cpp
    src/trezor-crypto/pbkdf2.cpp
    #src/trezor-crypto/rfc6979.cpp
    src/trezor-crypto/rand.cpp
    src/trezor-crypto/ripemd160.cpp
    src/trezor-crypto/script.cpp
    src/trezor-crypto/secp256k1.cpp
    src/trezor-crypto/sha2.cpp
    src/trezor-crypto/sha3.cpp
    )

set(HEADERS
    ${BTC_HEADERS}
    include/minter/bip39/crypto/common.h
    include/minter/bip39/crypto/sha256.h
    include/minter/bip39/crypto/hmac_sha256.h
    include/minter/bip39/crypto/hmac_sha512.h
    include/minter/bip39/crypto/sha512.h
    include/minter/bip39/bip39.h
    include/minter/bip39/wordlist.h
    include/minter/bip39/internal.h
    include/minter/bip39/bip39_core.h
    include/minter/bip39/config.h
    include/minter/bip39/uint256_t.hpp
    include/minter/bip39/PCGRand.hpp
    include/minter/bip39/utils.h
    include/minter/bip39/Bip39Mnemonic.h
    include/minter/bip39/HDKeyEncoder.h
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
    src/internal.cpp
    src/minter/utils.cpp
    src/minter/HDKeyEncoder.cpp
    src/minter/Bip39Mnemonic.cpp
    )

set(JNI_SOURCES
    src/java/network_minter_core_bip39_NativeBip39.h
    src/java/network_minter_core_bip39_NativeBip39.cpp
    src/java/network_minter_core_bip39_NativeHDKeyEncoder.h
    src/java/network_minter_core_bip39_NativeHDKeyEncoder.cpp
    src/java/nobject.h
    src/java/nobject.cpp
    )

# Core
if (ENABLE_SHARED)
	add_library(${PROJECT_NAME} SHARED ${SOURCES})
else ()
	add_library(${PROJECT_NAME} STATIC ${SOURCES})
endif ()
target_compile_options(${PROJECT_NAME} PUBLIC -Wno-shift-count-overflow -fPIC)
set_target_properties(${PROJECT_NAME} PROPERTIES LANGUAGE CXX)


target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs)

install(
	TARGETS bip39
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	COMPONENT main
)

install(
	DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/minter
	DESTINATION include
	COMPONENT main)

# GO
if (ENABLE_GO)
	set(BIP39_C_HEADERS
	    src/golang/bip39_config.h
	    src/golang/bip39.h
	    src/golang/hdkey_encoder.h
	    )

	set(BIP39_C_SOURCES
	    ${SOURCES}
	    ${BIP39_C_HEADERS}
	    src/golang/bip39.cpp
	    src/golang/bip39.cpp
	    src/golang/hdkey_encoder.cpp
	    )

	add_library(bip39_go ${BIP39_C_SOURCES})
	set_target_properties(bip39_go PROPERTIES
	                      PUBLIC_HEADER "${BIP39_C_HEADERS}"
	                      LANGUAGE C99)

	target_compile_options(bip39_go PRIVATE -Wno-shift-count-overflow -std=c++14 -fPIC)
	target_include_directories(bip39_go PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
	target_include_directories(bip39_go PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libs)
	target_include_directories(bip39_go PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/golang)

	install(
		TARGETS bip39_go
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib
		PUBLIC_HEADER DESTINATION include
		COMPONENT go-binding
	)
endif ()

# Android/JAVA
if (ENABLE_JNI)
	if (ANDROID_PLATFORM)
		add_definitions(-D__ANDROID__)
		add_library(${PROJECT_NAME}_jni SHARED ${JNI_SOURCES})
		target_link_libraries(${PROJECT_NAME}_jni ${PROJECT_NAME})
	else ()
		find_package(JNI)
		if (JNI_FOUND)
			add_library(${PROJECT_NAME}_jni SHARED ${JNI_SOURCES})
			target_link_libraries(${PROJECT_NAME}_jni ${PROJECT_NAME})
			target_link_libraries(${PROJECT_NAME}_jni ${JAVA_JVM_LIBRARY})
			target_include_directories(${PROJECT_NAME}_jni PRIVATE ${JNI_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/libs)

			install(
				TARGETS bip39_go
				LIBRARY DESTINATION lib
				ARCHIVE DESTINATION lib
				PUBLIC_HEADER DESTINATION include
				COMPONENT jni-binding
			)
		else ()
			message(STATUS "JNI not found")
		endif ()
	endif ()
endif ()

if (${RUN})
	add_executable(bip39dev src/main.cpp)
	target_link_libraries(bip39dev ${PROJECT_NAME})
	#	target_compile_options(bip39dev PUBLIC -fsanitize=address -m64 -pipe -ffunction-sections -fdata-sections  -O0 -g -fno-omit-frame-pointer)
endif ()


if (WITH_BIP39_TEST AND ENABLE_CONAN_BUILD)
	include(GoogleTest)

	# main test
	set(PROJECT_TEST_NAME "bip39_test_cxx")
	set(PROJECT_CC_TEST_NAME "bip39_test_cc")
	enable_testing()


	if (ENABLE_GO)
		add_executable(
			${PROJECT_CC_TEST_NAME}
			tests/minter_c_mnemonic_test.cpp
		)
		target_link_libraries(${PROJECT_CC_TEST_NAME} ${PROJECT_NAME})
		target_link_libraries(${PROJECT_CC_TEST_NAME} ${PROJECT_NAME}_go)
		target_link_libraries(${PROJECT_CC_TEST_NAME} CONAN_PKG::gtest)
		target_include_directories(${PROJECT_CC_TEST_NAME} PRIVATE ${BIP39_C_SOURCES})
		target_include_directories(${PROJECT_CC_TEST_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
		target_include_directories(${PROJECT_CC_TEST_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs)
		gtest_add_tests(TARGET ${PROJECT_CC_TEST_NAME})
	endif ()

	add_executable(
		${PROJECT_TEST_NAME}
		tests/minter_mnemonic_test.cpp
	)
	target_link_libraries(${PROJECT_TEST_NAME} ${PROJECT_NAME})
	target_link_libraries(${PROJECT_TEST_NAME} CONAN_PKG::gtest)
	target_include_directories(${PROJECT_TEST_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
	target_include_directories(${PROJECT_TEST_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs)
	gtest_add_tests(TARGET ${PROJECT_TEST_NAME})

endif ()


