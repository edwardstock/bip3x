cmake_minimum_required(VERSION 3.4)
project(trezor_crypto)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)


install(
	TARGETS trezor_crypto
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	PUBLIC_HEADER DESTINATION include
	COMPONENT trezor-crypto
)


