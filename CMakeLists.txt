cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(DNSAccServer)

###################################System#####################################
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED on)

# Build type
if (NOT CMAKE_BUILD_TYPE)
	set (CMAKE_BUILD_TYPE "Debug")

endif ()

if (NOT UNIX)
    message (FATAL_ERROR        "Operation system is not supported.")

endif ()

set (CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}"  CACHE STRING "${CMAKE_BUILD_TYPE}" FORCE)

if (GENERATOR_IS_MULTI_CONFIG OR CMAKE_CONFIGURATION_TYPES)
    set (CMAKE_CONFIGURATION_TYPES "${CMAKE_BUILD_TYPE}" CACHE STRING "${CMAKE_BUILD_TYPE}" FORCE)

endif()

message (STATUS "Build Type - ${CMAKE_BUILD_TYPE}.")
message (STATUS "Host System - ${CMAKE_HOST_SYSTEM_NAME}.")
message (STATUS "Host Processor - ${CMAKE_HOST_SYSTEM_PROCESSOR}.")
message (STATUS "Target System - ${CMAKE_SYSTEM_NAME}.")
message (STATUS "Target Processor - ${CMAKE_SYSTEM_PROCESSOR}.")

# Environment.
include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/include")

# Options
add_compile_options("-fPIC" "-fexceptions" "-g")

# Output path.
set (OUTPUT_SUB_DIR "${CMAKE_BUILD_TYPE}/${CMAKE_SYSTEM_NAME}/${CMAKE_SYSTEM_PROCESSOR}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY	"${CMAKE_CURRENT_SOURCE_DIR}/lib/${OUTPUT_SUB_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY	"${CMAKE_CURRENT_SOURCE_DIR}/lib/${OUTPUT_SUB_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY	"${CMAKE_CURRENT_SOURCE_DIR}/bin/${OUTPUT_SUB_DIR}")

link_directories ("${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

# Platform options.
# Debug/Release.
# Enable address sanitizer in debug mode if possible.
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"
			OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang"
			OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
		# Enable address sanitizer.
		add_compile_options (-fno-omit-frame-pointer -fsanitize=address)
		add_link_options (-fno-omit-frame-pointer -fsanitize=address)
		message (STATUS "Address sanitizer is enabled.")

	endif()

endif ()

###################################Dependencies################################

function (find_system_library)
    cmake_parse_arguments (ARGVAL "" "NAME" "HEADERS;LIBRARIES" ${ARGN})

    # Search header files.
    set (_dirs_found "")
    foreach (header IN LISTS ARGVAL_HEADERS)
        find_path (_ret  "${header}")
        if(_ret)
            message (STATUS "File \"${_ret}/${header}\" found.")
            list (APPEND _dirs_found "${_ret}")
            unset (_ret  CACHE)

        else ()
            unset (_ret  CACHE)
            unset (_dirs_found)
            message (FATAL_ERROR "Failed to find header file \"${header}\".")

        endif()

    endforeach ()

    set (${ARGVAL_NAME}_INCLUDE_DIRS "${_dirs_found}" CACHE STRING INTERNAL)
    unset (_dirs_found)

    # Search library files.
    set (_libs_found "")
    foreach (lib IN LISTS ARGVAL_LIBRARIES)
        find_library (_ret  "${lib}")
        if(_ret)
            message (STATUS "Library \"${_ret}\" found.")
            list (APPEND _libs_found "${_ret}")
            unset (_ret  CACHE)

        else ()
            unset (_ret  CACHE)
            unset (_libs_found)
            message (FATAL_ERROR "Failed to find library \"${lib}\".")

        endif()

    endforeach ()

    set (${ARGVAL_NAME}_LIBS "${_libs_found}" CACHE STRING INTERNAL)
    unset (_libs_found)

endfunction ()

# Doxygen
find_package (Doxygen REQUIRED)

#################################Targets#######################################
# Common
set (COMMON_TARGET "common")
file (GLOB_RECURSE COMMON_SOURCES
	"source/common/*.cc"
	"source/common/*.c"
	)

add_library (${COMMON_TARGET}    STATIC
    ${COMMON_SOURCES})

# Logger
set (LOGGER_TARGET "logger")
file (GLOB_RECURSE LOGGER_SOURCES
	"source/logger/*.cc"
	"source/logger/*.c"
	)

add_library (${LOGGER_TARGET}    STATIC
    ${LOGGER_SOURCES})
add_dependencies (${LOGGER_TARGET} 
    ${COMMON_TARGET})

# Service
option (BUILD_SERVICE "Build core service." ON)

if (BUILD_SERVICE)
## python3
    find_package (PythonLibs 3 REQUIRED)
    include_directories (${Python3_INCLUDE_DIRS})

    set (SERVICE_TARGET "dnsaccsrv")
    file (GLOB_RECURSE SERVICE_SOURCES
        "source/service/*.cc"
        "source/service/*.c"
        )

    add_executable (${SERVICE_TARGET} 
        ${SERVICE_SOURCES})
    add_dependencies (${SERVICE_TARGET} 
        ${COMMON_TARGET} 
        ${LOGGER_TARGET})
    target_link_libraries (${CORE_TARGET} 
        ${LOGGER_TARGET}
        ${COMMON_TARGET}
        ${Python3_LIBRARY}
        pthread
        )
endif ()

#Doc
add_custom_target("doc" ALL
	COMMAND				${DOXYGEN_EXECUTABLE}
	WORKING_DIRECTORY	${CMAKE_CURRENT_SOURCE_DIR})
