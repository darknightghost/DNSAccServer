cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(DNSAccServer)

###################################System#####################################
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
	message (STATUS "Current platform: Linux ")

else ()
	message (FATAL_ERROR "Not supported.")

endif ()

#Debug or release
if (CMAKE_BUILD_TYPE STREQUAL "Release")
	message (STATUS "Build type : Release")

	#Flags
	if (UNIX)
		add_compile_options("-Wall" "-Werror" "-O2")

	endif ()

else ()
	message (STATUS "Build type : Debug")
	set (CMAKE_BUILD_TYPE "Debug")

	#Flags
	if (UNIX)
		add_compile_options("-Wall" "-O0")

	endif ()

endif ()

#Options
add_compile_options("-fPIC" "-fexceptions" "-g")
set (OUTPUT_DIR "${CMAKE_SYSTEM_PROCESSOR}/${CMAKE_BUILD_TYPE}")

set (CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY	"${CMAKE_CURRENT_SOURCE_DIR}/symbols/${OUTPUT_DIR}")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY		"${CMAKE_CURRENT_SOURCE_DIR}/bin/${OUTPUT_DIR}")
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY		"${CMAKE_CURRENT_SOURCE_DIR}/lib/${OUTPUT_DIR}")
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY		"${CMAKE_CURRENT_SOURCE_DIR}/lib/${OUTPUT_DIR}")
include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/include")

###################################Dependencies################################
#Find and include header
function (find_and_inculde NAME)
	#Search file
	unset (FIND_RESULT CACHE)
	unset (FIND_RESULT_DIR_NAME CACHE)
	find_file (FIND_RESULT ${NAME})
	if (FIND_RESULT STREQUAL "FIND_RESULT-NOTFOUND")
		message (FATAL_ERROR "Cannot find file \"${NAME}\".")

	else ()
		message (STATUS "Found file \"${FIND_RESULT}\".")

		find_path (FIND_RESULT_DIR_NAME "${NAME}")

		#Include directory
		include_directories ("${FIND_RESULT_DIR_NAME}")

	endif ()
endfunction (find_and_inculde)

#Find library
macro (find_and_set_library VARNAME NAME)
	#Search library
	find_library (${VARNAME} "${NAME}")
	if (${VARNAME} STREQUAL "${VARNAME}-NOTFOUND")
		message (FATAL_ERROR "Cannot find library \"${NAME}\".")

	else ()
		message (STATUS "Found library \"${${VARNAME}}\".")

	endif ()
endmacro (find_and_set_library)

#libuv
find_and_inculde ("uv.h")
find_and_set_library (UV_LIB "uv")

#rapidjson
find_and_inculde ("rapidjson/rapidjson.h")

#python3
find_package (PythonLibs 3 REQUIRED)
include_directories (${Python3_INCLUDE_DIRS})

#sqlite3
find_and_inculde ("sqlite3.h")
find_and_set_library (SQLITE3_LIB "sqlite3")

#################################Targets#######################################
#Core
set (CORE_TARGET "dnsaccsrv")
file (GLOB_RECURSE CORE_SOURCES
	"source/core/*.cc"
	"source/core/*.c"
	)
add_executable (${CORE_TARGET} ${CORE_SOURCES})
target_link_libraries (${CORE_TARGET} 
	${UV_LIB}
	${Python3_LIBRARY}
	${SQLITE3_LIB}
	)
