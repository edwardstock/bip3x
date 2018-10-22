project(bip39)
cmake_minimum_required(VERSION 3.4)

set(CMAKE_CXX_STANDARD 14)

set(BTC_SRC
    src/crypto/common.h
    src/crypto/sha256.h
    src/crypto/sha256.cpp
    src/crypto/sha256_sse4.cpp
    src/crypto/hmac_sha256.h
    src/crypto/hmac_sha256.cpp
    src/crypto/hmac_sha512.h
    src/crypto/hmac_sha512.cpp
    src/crypto/sha512.h
    src/crypto/sha512.cpp
    )

if (WIN32)
	set(BTC_SRC
	    ${BTC_SRC}
	    src/crypto/win_endian.h
	    )
endif ()

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
    src/minter/PCGRand.hpp)

include_directories(libs/trezor-crypto)

set(JNI_SOURCES
    src/java/network_minter_core_bip39_NativeBip39.h
    src/java/network_minter_core_bip39_NativeBip39.cpp
    src/java/network_minter_core_bip39_NativeHDKeyEncoder.h
    src/java/network_minter_core_bip39_NativeHDKeyEncoder.cpp
    src/java/nobject.h
    src/java/nobject.cpp
    )

include_directories(src/)
include_directories(libs/uint256_t)


add_library(${PROJECT_NAME}_core SHARED ${SOURCES})
target_compile_options(${PROJECT_NAME}_core PUBLIC -Wno-shift-count-overflow)
add_subdirectory(libs/trezor-crypto)
target_link_libraries(${PROJECT_NAME}_core trezor_crypto)


if (NOT DISABLE_JNI)
	if (ANDROID_PLATFORM)
		add_definitions(-D__ANDROID__)
		add_library(${PROJECT_NAME}_jni SHARED ${JNI_SOURCES})
		target_link_libraries(${PROJECT_NAME}_jni ${PROJECT_NAME}_core)
	else ()
		find_package(JNI)
		if (JNI_FOUND)
			add_library(${PROJECT_NAME}_jni SHARED ${JNI_SOURCES})
			target_link_libraries(${PROJECT_NAME}_jni ${PROJECT_NAME}_core)
			target_link_libraries(${PROJECT_NAME}_jni ${JAVA_JVM_LIBRARY})
			target_include_directories(${PROJECT_NAME}_jni PRIVATE ${JNI_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/src)
		else ()
			message(WARN "JNI not found")
		endif ()
	endif ()
endif ()

if (${RUN})
	add_executable(bip39dev src/main.cpp)
	target_link_libraries(bip39dev ${PROJECT_NAME}_core)
endif ()

if (${ENABLE_TEST})
	set(PROJECT_TEST_NAME "bip39_test")
	enable_testing()
	add_definitions(-DBIP39_TESTING=1)
	set(GTEST_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/googletest/googletest)

	include_directories(${GTEST_SOURCE_DIR}/include ${GTEST_SOURCE_DIR})
	add_subdirectory(${GTEST_SOURCE_DIR})

	if (WIN32)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
	elseif (APPLE)
		add_definitions(-DGTEST_USE_OWN_TR1_TUPLE)
		add_definitions(-D__GLIBCXX__)
	endif ()

	add_executable(
		${PROJECT_TEST_NAME}
		tests/minter_mnemonic_test.cpp
	)

	target_compile_options(${PROJECT_TEST_NAME} PRIVATE -Wno-missing-field-initializers)

	target_link_libraries(${PROJECT_TEST_NAME} ${PROJECT_NAME}_core)
	target_link_libraries(${PROJECT_TEST_NAME} gtest gtest_main)

endif ()