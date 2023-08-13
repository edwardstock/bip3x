set(C_HEADERS
    include/cbip3x/cbip3x_config.h
    include/cbip3x/cbip3x.h
    include/cbip3x/cbip3x_hdkey_encoder.h
)
set(C_SOURCES
    src/cbip3x/cbip3x.cpp
    src/cbip3x/cbip3x_hdkey_encoder.cpp
)

if (bip3x_BUILD_SHARED_LIBS)
	message(STATUS "Build C shared library")
	add_library(${LIB_NAME_C} SHARED ${SOURCES} ${C_SOURCES} ${C_HEADERS})
else ()
	add_library(${LIB_NAME_C} STATIC ${SOURCES} ${C_SOURCES} ${C_HEADERS})
endif ()
# link toolbox again, as we don't link main library, only using sources directly
target_link_libraries(${LIB_NAME_C} toolbox::toolbox)

if (bip3x_USE_OPENSSL_RANDOM)
	if (CMAKE_CONAN_BUILD)
		target_link_libraries(${LIB_NAME_C} openssl::openssl)
	else ()
		target_link_libraries(${LIB_NAME_C} OpenSSL::Crypto)
	endif ()
endif ()

target_include_directories(${LIB_NAME_C} PUBLIC
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                           $<INSTALL_INTERFACE:include>
)
target_include_directories(${LIB_NAME_C} PRIVATE ${CMAKE_SOURCE_DIR}/include)

## INSTALL CONFIG
install(
	DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/cbip3x
	DESTINATION include
)
