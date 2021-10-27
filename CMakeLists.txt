include(${CMAKE_SOURCE_DIR}/MicroMaxCmakeA10.cmake)
cmake_minimum_required(VERSION 3.16)

project(master_slave)

set(CMAKE_BUILD_TYPE Debug)
cmake_minimum_required(VERSION 3.16)

project (master_slave)

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -std=c++14")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")


include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
    
set(SOURCES_MASTER
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/master.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/shared_memory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/shm.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pipe.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/cobs.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/container_requests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/modbus.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/request_handler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/fifo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/reg_file.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/client.cpp)

option(FORK_PIPE "Build programm only module FORK and PIPE" ON)
option(FORK_SHM_SEMAPHORE "Build programm only module FORK, SHM, SEMAPHORE" OFF)
option(DEBUG "Build programm with debug information" OFF)
option(TIME "Build programm with time information" ON)

if(FORK_PIPE)
    add_compile_definitions(FORK_PIPE)
endif(FORK_PIPE)

if(FORK_SHM_SEMAPHORE)
    add_compile_definitions(FORK_SHM_SEMAPHORE)
endif(FORK_SHM_SEMAPHORE)

if(DEBUG)
    add_compile_definitions(DEBUG)
endif(DEBUG)

if(TIME)
    add_compile_definitions(TIME)
endif(TIME)

add_executable(master ${SOURCES_MASTER})