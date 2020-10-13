# Detect Redhat or Debian platform
# The following variable will be set
# $PLATFORM		Debian	Redhat		none
# $DIST_NAME	 deb	rhel	fc	none
# $BIT_MODE		32|64
#
# $OS_NAME debian | ubuntu | fedora | centos
#

if (NOT WINDOWS)
	if (${CMAKE_SYSTEM_PROCESSOR} MATCHES i386|i586|i686)
		set(BIT_MODE "32")
	else ()
		set(BIT_MODE "64")
	endif ()
	set(OS_ARCH ${CMAKE_SYSTEM_PROCESSOR})
	if (BIT_MODE STREQUAL "64")
		set(OS_ARCH "amd64")
	endif ()

	if (EXISTS "/etc/debian_version")
		set(IS_DEBIAN ON)
		set(OS_NAME debian)
		execute_process(
			COMMAND lsb_release -c -s
			OUTPUT_VARIABLE DEB_DIST_NAME
		)
		string(REGEX REPLACE "\n$" "" DEB_DIST_NAME "${DEB_DIST_NAME}")
	endif (EXISTS "/etc/debian_version")

	if (EXISTS "/etc/redhat-release")
		set(IS_REDHAT ON)
		set(OS_ARCH ${CMAKE_SYSTEM_PROCESSOR})

		if (EXISTS "/etc/fedora-release")
			set(IS_FEDORA ON)
			set(OS_NAME fedora)
			execute_process(
				COMMAND rpm -E %{fedora}
				OUTPUT_VARIABLE RH_MAJOR_VERSION
			)
			string(REGEX REPLACE "\n$" "" RH_MAJOR_VERSION "${RH_MAJOR_VERSION}")
			set(RH_MAJOR_VERSION ${RH_MAJOR_VERSION})
			set(RH_MAJOR_NAME "fc${RH_MAJOR_VERSION}")
		elseif (EXISTS "/etc/centos-release")
			set(IS_CENTOS ON)
			set(OS_NAME centos)
			execute_process(
				COMMAND rpm -E %{rhel}
				OUTPUT_VARIABLE RH_MAJOR_VERSION
			)
			string(REGEX REPLACE "\n$" "" RH_MAJOR_VERSION "${RH_MAJOR_VERSION}")
			set(RH_MAJOR_VERSION ${RH_MAJOR_VERSION})
			set(RH_MAJOR_NAME "el${RH_MAJOR_VERSION}")
		endif ()

	endif (EXISTS "/etc/redhat-release")
endif (NOT WINDOWS)