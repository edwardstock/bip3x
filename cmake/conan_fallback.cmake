function (conan_fallback)
	set(options)
	set(single NAME LOCAL_INCLUDE_DIR SUBDIR SYSTEM_INCLUDE_DIR TARGET_NAME LOCAL_LIBS_PREFIX)
	set(multi PROPERTIES)
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
		list(LENGTH FF_PROPERTIES OLD_PROPS_SZ)
		set(OLD_PROPS)
		if (${OLD_PROPS_SZ} GREATER 0)
			math(EXPR OLD_PROPS_SZ "(${OLD_PROPS_SZ} / 2)-1")

			set(it 0)
			foreach (idx RANGE 0 ${OLD_PROPS_SZ})
				math(EXPR idx "${idx} + ${it}")
				list(GET FF_PROPERTIES ${idx} _PKEY)
				list(APPEND OLD_PROPS "${_PKEY}")
				list(APPEND OLD_PROPS "${${_PKEY}}")

				math(EXPR it "${it}+1")
			endforeach ()

			set(it 0)
			foreach (idx RANGE 0 ${OLD_PROPS_SZ})
				math(EXPR idx "${idx} + ${it}")
				math(EXPR _next_idx "${idx} + 1")
				list(GET FF_PROPERTIES ${idx} _PKEY)
				list(GET FF_PROPERTIES ${_next_idx} _PVALUE)
				list(APPEND PRINT_PROPS_${FF_NAME} "-D${_PKEY}=${_PVALUE}")
				set(${_PKEY} ${_PVALUE} CACHE BOOL "" FORCE)
				math(EXPR it "${it}+1")
			endforeach ()
		endif ()

		add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/${LOCAL_LIBS_PREFIX}/${LOCAL_SUBDIR_${FF_NAME}})

		if (${OLD_PROPS_SZ} GREATER 0)
			math(EXPR OLD_PROPS_SZ "(${OLD_PROPS_SZ} / 2)-1")
			set(it 0)
			foreach (idx RANGE 0 ${OLD_PROPS_SZ})
				math(EXPR idx "${idx} + ${it}")
				math(EXPR _next_idx "${idx} + 1")
				list(GET OLD_PROPS ${idx} _PKEY)
				list(GET OLD_PROPS ${_next_idx} _PVALUE)
				set(${_PKEY} ${_PVALUE} CACHE BOOL "" FORCE)
				math(EXPR it "${it}+1")
			endforeach ()

		endif ()

		# if set local path where includes stored, adding it to global includes
		if (FF_LOCAL_INCLUDE_DIR)
			set(${FF_NAME}_INCLUDE_DIR ${FF_LOCAL_INCLUDE_DIR})
		else ()
			set(${FF_NAME}_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${LOCAL_LIBS_PREFIX}/${LOCAL_SUBDIR_${FF_NAME}}/include)
		endif ()
		include_directories(${${FF_NAME}_INCLUDE_DIR})
		add_library(CONAN_PKG::${FF_NAME} ALIAS ${LOCAL_TARGET_NAME})

		message(STATUS "off-conan: ${FF_NAME} not found.
		Using directory: ${CMAKE_CURRENT_SOURCE_DIR}/${LOCAL_LIBS_PREFIX}/${LOCAL_SUBDIR_${FF_NAME}}
		Target:          ${FF_NAME}
		Include:         ${${FF_NAME}_INCLUDE_DIR}
		Properties:      ${PRINT_PROPS_${FF_NAME}}")
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