set(JNI_SOURCES
    src/jni/com_edwardstock_bip3x_BIP3X.h
    src/jni/com_edwardstock_bip3x_BIP3X.cpp
    src/jni/com_edwardstock_bip3x_HDKeyEncoder.h
    src/jni/com_edwardstock_bip3x_HDKeyEncoder.cpp
    src/jni/nobject.h
    src/jni/nobject.cpp
)
# jni should be always shared
add_library(${LIB_NAME_JNI} SHARED ${JNI_SOURCES})

if (ANDROID_PLATFORM)
	add_definitions(-D__ANDROID__)
else ()
	find_package(Java COMPONENTS Development)
	find_package(JNI BYPASS_PROVIDER REQUIRED)
	if (JNI_FOUND)
		target_link_libraries(${LIB_NAME_JNI} PRIVATE ${JAVA_JVM_LIBRARY})
		target_include_directories(${LIB_NAME_JNI} PRIVATE ${JNI_INCLUDE_DIRS} ${JAVA_INCLUDE_PATH2} ${JAVA_AWT_INCLUDE_PATH})
	else ()
		message(FATAL_ERROR "JNI not found ${JAVA_JVM_LIBRARY} ${JNI_INCLUDE_DIRS}")
	endif ()
endif ()

target_link_libraries(${PROJECT_NAME}_jni PRIVATE ${LIB_NAME_MAIN})
