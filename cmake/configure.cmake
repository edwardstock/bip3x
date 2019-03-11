include(CheckIncludeFile)
include(CheckIncludeFiles)
include(CheckFunctionExists)
include(CheckSymbolExists)
include(TestBigEndian)

check_include_file("dlfcn.h" HAVE_DLFCN_H)
check_include_file("fcntl.h" HAVE_FCNTL_H)
check_include_file("inttypes.h" HAVE_INTTYPES_H)
check_include_file("memory.h" HAVE_MEMORY_H)
check_include_file("stdint.h" HAVE_STDINT_H)
check_include_file("stdlib.h" HAVE_STDLIB_H)
check_include_file("strings.h" HAVE_STRINGS_H)
check_include_file("string.h" HAVE_STRING_H)
check_include_file("sys/stat.h" HAVE_SYS_STAT_H)
check_include_file("sys/types.h" HAVE_SYS_TYPES_H)
check_include_file("unistd.h" HAVE_UNISTD_H)

check_symbol_exists("uint8_t" "stdint.h;inttypes.h" HAVE_UINT8_TYPE)
check_symbol_exists("uint32_t" "stdint.h;inttypes.h" HAVE_UINT32_TYPE)
check_symbol_exists("uint64_t" "stdint.h;inttypes.h" HAVE_UINT64_TYPE)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/src/golang/bip39_config.h.in
	${CMAKE_CURRENT_SOURCE_DIR}/src/golang/bip39_config.h
)