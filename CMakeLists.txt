project(bip39)
cmake_minimum_required(VERSION 3.4)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -pedantic")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -g0 -O2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c99-extensions -Wno-unused-function -Wno-unused-const-variable")

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/configure.cmake)

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


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/trezor-crypto)

set(JNI_SOURCES
    src/java/network_minter_core_bip39_NativeBip39.h
    src/java/network_minter_core_bip39_NativeBip39.cpp
    src/java/network_minter_core_bip39_NativeHDKeyEncoder.h
    src/java/network_minter_core_bip39_NativeHDKeyEncoder.cpp
    src/java/nobject.h
    src/java/nobject.cpp
    )

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/uint256_t)

# Core
add_library(${PROJECT_NAME}_core SHARED ${SOURCES})
target_compile_options(${PROJECT_NAME}_core PUBLIC -Wno-shift-count-overflow -fPIC)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/trezor-crypto)
target_link_libraries(${PROJECT_NAME}_core trezor_crypto)

# GO
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
target_compile_options(bip39_go PRIVATE -Wno-shift-count-overflow -std=c++14)
target_link_libraries(bip39_go trezor_crypto)
#set(CGO_FLAGS "CGO_FLAGS=\"-std=c11 -I${CMAKE_CURRENT_SOURCE_DIR}/src/golang\" CGO_LDFLAGS=\"-L${CMAKE_BINARY_DIR} -L${CMAKE_BINARY_DIR}/libs/trezor-crypto -lstdc++ -lbip39_go -ltrezor_crypto\"")
#add_custom_target(bip39-go-build
#                  COMMAND bash -c "${CGO_FLAGS} go build"
#                  DEPENDS bip39_go
#                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/golang/bip39
#                  )
#
#add_custom_target(bip39-go-test
#                  COMMAND bash -c "${CGO_FLAGS} go test"
#                  DEPENDS bip39-go-build
#                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/golang/bip39
#                  )


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
			message(STATUS "JNI not found")
		endif ()
	endif ()
endif ()

if (${RUN})
	add_executable(bip39dev src/main.cpp)
	target_link_libraries(bip39dev ${PROJECT_NAME}_core)
	#	target_compile_options(bip39dev PUBLIC -fsanitize=address -m64 -pipe -ffunction-sections -fdata-sections  -O0 -g -fno-omit-frame-pointer)
endif ()

# yum install java-1.8.0-openjdk java-1.8.0-openjdk-devel
if (WITH_TEST)

	# main test
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

	# native C test
	add_executable(bip39_c_test
	               tests/minter_c_mnemonic_test.cpp
	               )

	target_compile_options(bip39_c_test PRIVATE -Wno-missing-field-initializers)
	target_link_libraries(bip39_c_test ${PROJECT_NAME}_core)
	target_link_libraries(bip39_c_test ${PROJECT_NAME}_go)
	target_link_libraries(bip39_c_test gtest gtest_main)
	target_include_directories(bip39_c_test PRIVATE ${BIP39_C_SOURCES})
endif ()


install(
	TARGETS bip39_go
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	PUBLIC_HEADER DESTINATION include
)

install(FILES ${BIP39_C_HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include)