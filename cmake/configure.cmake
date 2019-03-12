include(CheckIncludeFile)
include(CheckIncludeFiles)
include(CheckFunctionExists)
include(CheckSymbolExists)
include(CheckCXXSymbolExists)
include(TestBigEndian)

check_include_file("sys/types.h" HAVE_SYS_TYPES_H)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/src/golang/bip39_config.h.in
	${CMAKE_CURRENT_SOURCE_DIR}/src/golang/bip39_config.h
)