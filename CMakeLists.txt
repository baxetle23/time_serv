include(${CMAKE_SOURCE_DIR}/MicroMaxCmakeA10.cmake)
cmake_minimum_required(VERSION 3.16)

project(shared_memory)

set(CMAKE_BUILD_TYPE Debug)
cmake_minimum_required(VERSION 3.16)

project (shared_memory)

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wextra -std=c++14")
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
    ${CMAKE_CURRENT_SOURCE_DIR}/src/client.cpp)

option(FORK_PIPE "Build programm only module FORK and PIPE" ON)
option(FORK_SHM_SEMAPHORE "Build programm only module FORK, SHM, SEMAPHORE" OFF)

if(FORK_PIPE)
    add_compile_definitions(FORK_PIPE)
endif(FORK_PIPE)

if(FORK_SHM_SEMAPHORE)
    add_compile_definitions(FORK_SHM_SEMAPHORE)
endif(FORK_SHM_SEMAPHORE)

add_executable(master ${SOURCES_MASTER})