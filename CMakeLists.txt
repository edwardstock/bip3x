cmake_minimum_required(VERSION 3.4)
project(bip39
        VERSION 2.0.0
        DESCRIPTION "Native BIP39 Mnemonic implementation (with JNI and pure C bindings)"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY lib)
set(CMAKE_PDB_OUTPUT_DIRECTORY lib)

set(ENABLE_SHARED OFF CACHE BOOL "Build shared core library instead of static")
set(ENABLE_TEST OFF CACHE BOOL "Compile unit google tests")
set(ENABLE_BIP39_JNI ON CACHE BOOL "Enable compiling for android/jre")
set(ENABLE_BIP39_C ON CACHE BOOL "Enable compiling pure C for other language bindings")
set(ENABLE_CONAN ON CACHE BOOL "Enable conan dependencies instead of system")

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
elseif (MINGW)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Wno-unknown-pragmas")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -pedantic")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -g0 -O2")
else ()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-shift-count-overflow -fPIC -Wno-c99-extensions")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -pedantic -Wunused-function")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -g0 -O2")
endif ()


list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(ConanInit)
include(conan_fallback)

if (ENABLE_CONAN)
	add_conan_remote(bincrafters https://api.bintray.com/conan/bincrafters/public-conan)
	add_conan_remote(edwardstock https://api.bintray.com/conan/edwardstock/conan-public)
	conan_init()
else ()
	conan_fallback(NAME toolbox PROPERTIES
	               ENABLE_SHARED OFF
	               ENABLE_TEST OFF)
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


if (ENABLE_SHARED)
	message(STATUS "Build bip39 shared libs")
	add_library(${PROJECT_NAME} SHARED ${SOURCES})
else ()
	message(STATUS "Build bip39 static libs")
	add_library(${PROJECT_NAME} STATIC ${SOURCES})
endif ()

target_link_libraries(${PROJECT_NAME} CONAN_PKG::toolbox)

if (MSVC)
	set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY LINK_FLAGS /version:${CMAKE_PROJECT_VERSION_MAJOR}.${CMAKE_PROJECT_VERSION_MINOR})
endif ()

set_target_properties(${PROJECT_NAME} PROPERTIES LANGUAGE CXX)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs)

install(
	TARGETS bip39
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	COMPONENT main
)

install(
	DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/bip3x
	DESTINATION include
	COMPONENT main)

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
		add_library(cbip39 SHARED ${BIP39_C_SOURCES})
	else ()
		add_library(cbip39 STATIC ${BIP39_C_SOURCES})
	endif ()
	# link toolbox again, as we don't link main library, only using sources directly
	target_link_libraries(cbip39 CONAN_PKG::toolbox)

	set_target_properties(cbip39 PROPERTIES
	                      PUBLIC_HEADER "${BIP39_C_HEADERS}"
	                      LANGUAGE C99)

	target_include_directories(cbip39 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
	target_include_directories(cbip39 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libs)
	target_include_directories(cbip39 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/bindings)

	install(
		TARGETS cbip39
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib
		PUBLIC_HEADER DESTINATION include
		COMPONENT c-binding
	)
endif ()

# Android/JAVA
if (ENABLE_BIP39_JNI)
	if (ANDROID_PLATFORM)
		add_definitions(-D__ANDROID__)
		if (ENABLE_SHARED)
			add_library(${PROJECT_NAME}_jni SHARED ${JNI_SOURCES})
		else ()
			add_library(${PROJECT_NAME}_jni STATIC ${JNI_SOURCES})
		endif ()
		target_link_libraries(${PROJECT_NAME}_jni ${PROJECT_NAME})
	else ()
		find_package(JNI)
		if (JNI_FOUND)
			if (ENABLE_SHARED)
				add_library(${PROJECT_NAME}_jni SHARED ${JNI_SOURCES})
			else ()
				add_library(${PROJECT_NAME}_jni STATIC ${JNI_SOURCES})
			endif ()
			target_link_libraries(${PROJECT_NAME}_jni ${PROJECT_NAME})
			target_link_libraries(${PROJECT_NAME}_jni ${JAVA_JVM_LIBRARY})
			target_include_directories(${PROJECT_NAME}_jni PRIVATE ${JNI_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/libs)

			install(
				TARGETS ${PROJECT_NAME}_jni
				RUNTIME DESTINATION bin
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

if (ENABLE_PVS)
	include(cmake/PVS-Studio.cmake)

	pvs_studio_add_target(TARGET ${PROJECT_NAME}.analyze ALL
	                      OUTPUT FORMAT errorfile
	                      CONFIG ${CMAKE_CURRENT_SOURCE_DIR}/pvs.cfg
	                      ANALYZE ${PROJECT_NAME}
	                      LOG target_main.err)
endif ()


if (ENABLE_TEST)
	conan_fallback(NAME gtest LOCAL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/gtest/googletest/include)
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
	target_link_libraries(${PROJECT_TEST_NAME} ${PROJECT_NAME})

	if (ENABLE_BIP39_C)
		target_link_libraries(${PROJECT_TEST_NAME} cbip39)
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


