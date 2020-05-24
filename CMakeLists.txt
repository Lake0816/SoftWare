# CMake Minimum version number required
CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

# Set Project Name
PROJECT(SoftWare)

# Set Config.h.in, Config.h Path
#configure_file (
#  "${PROJECT_SOURCE_DIR}/config/config.h.in"
#  "${PROJECT_BINARY_DIR}/config.h"
#  ) 

SET(CMAKE_C_FLAGS "-g -Wall -O2")

SET(CMAKE_CXX_FLAGS "-std=c++11 -g -Wall -02 ${CMAKE_CXX_FLAGS}")

SET(CMAKE_OUTPUT_DIR ${PROJECT_SOURCE_DIR}/output)

SET(CMAKE_BUILD_TYPE "debug")
# Set Bin Path
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_OUTPUT_DIR}/bin)
MESSAGE(STATUS "Bin Path: " ${EXECUTABLE_OUTPUT_PATH})

SET(LIBRARY_OUTPUT_PATH ${CMAKE_OUTPUT_DIR}/lib)
MESSAGE(STATUS "Lib Path: " ${LIBRARY_OUTPUT_PATH})

SET(CONFIG_FILE_PATH ${PROJECT_SOURCE_DIR}/config)

CONFIGURE_FILE(
    "${CONFIG_FILE_PATH}/config.h.in"
    "${PROJECT_SOURCE_DIR}/public/config.h"
)

# Only set default value, not set to config.h file
OPTION(CONFIG_COMMON_CRC "Use Crc Check Module" ON)

OPTION(CONFIG_COMMON_SOCKET "Use Socket Module" ON)

OPTION(CONFIG_COMMON_LINUX_SHM "Use linux share memory Module" ON)

OPTION(CONFIG_COMMON_LINUX_MSG_QUEUE "Use linux msg queue module" ON)

OPTION(CONFIG_COMMON_LINUX_SEM "Use linux shem" ON)

ADD_SUBDIRECTORY(com/src)
MESSAGE(STATUS "Add directory com/src")

if (CMAKE_BUILD_TYPE MATCHES "debug")
	ADD_SUBDIRECTORY(demo/Tcp)
	MESSAGE(STATUS "Add directory demo")
elseif (CMAKE_BUILD_TYPE MATCHES "release")	
	ADD_SUBDIRECTORY(src)
	MESSAGE(STATUS "Add directory src")
endif ()


