if (CMAKE_CONAN_BUILD)
	message(STATUS "Finding gtest...")
	find_package(GTest CONFIG REQUIRED)
else ()
	fetchcontent_declare(
		GTest
		GIT_REPOSITORY https://github.com/google/googletest.git
		GIT_TAG v1.13.0
	)
	set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
	set(gtest_force_shared_crt ${bip3x_BUILD_SHARED_LIBS} CACHE BOOL "" FORCE)
	fetchcontent_makeavailable(GTest)
endif ()

if (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++14")
elseif (APPLE)
	add_definitions(-DGTEST_USE_OWN_TR1_TUPLE)
	add_definitions(-D__GLIBCXX__)
endif ()

include(GoogleTest)

# main test
set(PROJECT_TEST_NAME "${PROJECT_NAME}-test")
enable_testing()

set(TEST_SOURCES
    tests/main.cpp
    tests/minter_cxx_test.cpp
    tests/issue_11_test.cpp)

if (bip3x_BUILD_C_BINDINGS)
	set(TEST_SOURCES ${TEST_SOURCES}
	    tests/minter_cc_test.cpp)
endif ()

add_executable(${PROJECT_TEST_NAME} ${TEST_SOURCES})
target_link_libraries(${PROJECT_TEST_NAME} ${LIB_NAME_MAIN})

if (bip3x_BUILD_C_BINDINGS)
	target_link_libraries(${PROJECT_TEST_NAME} ${LIB_NAME_C})
	target_include_directories(${PROJECT_TEST_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src/cbip3x)
endif ()

if (CMAKE_CONAN_BUILD)
	target_link_libraries(${PROJECT_NAME}-test GTest::GTest)
else ()
	target_link_libraries(${PROJECT_NAME}-test gtest)
endif ()
target_include_directories(${PROJECT_TEST_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(${PROJECT_TEST_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/libs)
gtest_add_tests(TARGET ${PROJECT_TEST_NAME})

if (bip3x_ENABLE_SANITIZER)
	target_link_libraries(${PROJECT_TEST_NAME} asan)
	target_compile_options(${PROJECT_TEST_NAME} PUBLIC -fsanitize=address -m64 -pipe -ffunction-sections -fdata-sections -O0 -g -fno-omit-frame-pointer)
endif ()
