cmake_minimum_required(VERSION 3.10)
project(record_bag)

set(CMAKE_BUILD_TYPE Debug)

file (GLOB_RECURSE SRC_FILE 
	"${CMAKE_SOURCE_DIR}/src/*.c"	
)

file (GLOB_RECURSE INC_FILE
	"${CMAKE_SOURCE_DIR}/inc"
)

add_executable(${PROJECT_NAME} ${SRC_FILE})


