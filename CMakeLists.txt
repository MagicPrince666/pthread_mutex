CMAKE_MINIMUM_REQUIRED(VERSION 3.1)
option(EXAMPLE "shall we complie example" on)

include_directories(${CMAKE_SOURCE_DIR})

PROJECT(CppDemo)

#SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_BUILD_TYPE "Release")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -O0 -Wall -g")
SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -Os -Wall")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -std=c++11 -O0 -Wall -g")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -std=c++11 -Os -Wall")

MESSAGE(STATUS ${CMAKE_SYSTEM_NAME})

IF (WIN32)
	MESSAGE(STATUS "Now is windows.")
ELSEIF (APPLE)
	MESSAGE(STATUS "Now is Apple systens.")
ELSEIF (UNIX)
	MESSAGE(STATUS "Now is UNIX-like OS's.")
ENDIF ()

set(EXEC_NAME cdemo)

add_executable(${EXEC_NAME}
	main.c
	ringbuf.c
)

target_link_libraries(${EXEC_NAME}
	pthread
)

