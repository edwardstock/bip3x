function (conan_fallback)
	set(options)
	set(single NAME LOCAL_INCLUDE_DIR SUBDIR SYSTEM_INCLUDE_DIR TARGET_NAME LOCAL_LIBS_PREFIX)
	set(multi)
	cmake_parse_arguments("FF" "${options}" "${single}" "${multi}" ${ARGN})

	if (TARGET CONAN_PKG::${FF_NAME})
		return()
	endif ()

	set(LOCAL_LIBS_PREFIX "libs")
	if (FF_LOCAL_LIBS_PREFIX)
		set(LOCAL_LIBS_PREFIX ${FF_LOCAL_LIBS_PREFIX})
	endif ()

	set(LOCAL_SUBDIR_${FF_NAME} ${FF_NAME})
	if (FF_SUBDIR)
		set(LOCAL_SUBDIR_${FF_NAME} ${FF_SUBDIR})
	endif ()


	#	Check library name is set
	if (NOT FF_NAME)
		message(FATAL_ERROR "off-conan: NAME required")
	endif ()

	#	if defined subdirectory target name use it instead of NAME
	set(LOCAL_TARGET_NAME ${FF_NAME})
	if (FF_TARGET_NAME)
		set(LOCAL_TARGET_NAME ${FF_TARGET_NAME})
	endif ()

	#	Trying to find system-wide library
	find_library(FF_FIND_LIB_${FF_NAME} ${FF_NAME})

	#	If not found, trying to add subdirectory
	if (NOT FF_FIND_LIB_${FF_NAME})
		add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/${LOCAL_LIBS_PREFIX}/${LOCAL_SUBDIR_${FF_NAME}})
		#		if set local path where includes stored, adding it to global includes
		if (FF_LOCAL_INCLUDE_DIR)
			set(${FF_NAME}_INCLUDE_DIR ${FF_LOCAL_INCLUDE_DIR})
		else ()
			set(${FF_NAME}_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${LOCAL_LIBS_PREFIX}/${FF_NAME}/include)
		endif ()
		include_directories(${FF_NAME}_INCLUDE_DIR)
		add_library(CONAN_PKG::${FF_NAME} ALIAS ${LOCAL_TARGET_NAME})

		message(STATUS "off-conan: ${FF_NAME} not found.
		Fallback to libs directory ${CMAKE_CURRENT_SOURCE_DIR}/${LOCAL_LIBS_PREFIX}/${FF_NAME};
		target: ${FF_NAME}
		include: ${${FF_NAME}_INCLUDE_DIR}")
	else ()
		set(${FF_NAME}_LIBRARIES "")
		set(${FF_NAME}_LIBRARIES ${FF_FIND_LIB_${FF_NAME}})
		set(${FF_NAME}_INCLUDE_DIR "")
		set(FIND_HEADER_NAMES ${FF_NAME}.h ${FF_NAME}_config.h)
		set(FIND_HEADER_SUFFIXES include ${FF_NAME} include/${FF_NAME})
		find_path(FF_FIND_LIB_INCLUDE_DIRS
		          NAMES ${FIND_HEADER_NAMES}
		          PATHS
		          ENV ${FF_NAME}_ROOT
		          PATH_SUFFIXES ${FIND_HEADER_SUFFIXES}
		          )

		if (FF_FIND_LIB_INCLUDE_DIRS)
			set(${FF_NAME}_INCLUDE_DIR ${FF_FIND_LIB_INCLUDE_DIRS})
		elseif (FF_SYSTEM_INCLUDE_DIR)
			set(${FF_NAME}_INCLUDE_DIR ${FF_SYSTEM_INCLUDE_DIR})
		endif ()

		include_directories(${FF_NAME}_INCLUDE_DIR)
		message(STATUS "off-conan: ${FF_NAME} found ${FF_FIND_LIB_${FF_NAME}}.
		libs: ${${FF_NAME}_LIBRARIES}
		include: ${${FF_NAME}_INCLUDE_DIR}")

		add_library(CONAN_PKG::${FF_NAME} STATIC IMPORTED)
		set_target_properties(CONAN_PKG::${FF_NAME} PROPERTIES IMPORTED_LOCATION ${FF_FIND_LIB_${FF_NAME}})
	endif ()
endfunction ()