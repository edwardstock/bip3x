include(CMakeParseArguments)
function (glue_archives)
	set(options)
	set(single SOURCE TARGET)
	set(multi DEPENDS)
	cmake_parse_arguments("GLUE" "${options}" "${single}" "${multi}" ${ARGN})

	if (NOT GLUE_SOURCE)
		message(FATAL_ERROR "SOURCE not set")
	endif ()
	if (NOT GLUE_TARGET)
		set(GLUE_TARGET ${GLUE_SOURCE}-merged)
	endif ()
	if (NOT GLUE_DEPENDS)
		message(FATAL_ERROR "You must pass dependant libraries")
	endif ()

	set(G_TMP ${CMAKE_BINARY_DIR}/lib_merge)
	set(G_CMDS rm -rf ${CMAKE_BINARY_DIR}/lib_merge
	    && mkdir -p ${G_TMP} && cd ${G_TMP}
	    && ar -x $<TARGET_FILE:${GLUE_SOURCE}>)

	foreach (libname ${GLUE_DEPENDS})
		message(STATUS "Check location for ${libname}")
		get_target_property(G_DEP_LOCATION ${libname} IMPORTED_LOCATION)
		#		message(FATAL_ERROR ${G_DEP_LOCATION})
		if (G_DEP_LOCATION-NOTFOUND)
			set(G_CMDS ${G_CMDS} && ar -x $<TARGET_FILE:${libname}>)
		else ()
			set(G_CMDS ${G_CMDS} && ar -x ${G_DEP_LOCATION})
		endif ()
	endforeach ()

	set(G_CMDS ${G_CMDS} && ar -qcs lib${GLUE_TARGET}.a *.o
	    && rm -rf *.o
	    && mkdir -p ${CMAKE_BINARY_DIR}/lib
	    && mv lib${GLUE_TARGET}.a ${CMAKE_BINARY_DIR}/lib
	    )


	add_custom_target(${GLUE_SOURCE}-ar
	                  COMMAND ${G_CMDS}
	                  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	                  DEPENDS ${GLUE_SOURCE} ${GLUE_DEPENDS}
	                  )

	set(GLUE_DEPENDENT_TARGET ${GLUE_SOURCE}-ar)

	add_library(${GLUE_TARGET} STATIC IMPORTED GLOBAL)
	add_dependencies(${GLUE_TARGET} ${GLUE_SOURCE}-ar)
	set_target_properties(${GLUE_TARGET} PROPERTIES
	                      IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/lib/lib${GLUE_TARGET}.a)

endfunction (glue_archives)