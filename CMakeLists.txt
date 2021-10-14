cmake_minimum_required(VERSION 3.16)

project (shared_memory)

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wextra")
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
    

add_executable(master ${SOURCES_MASTER})

# include(${CMAKE_SOURCE_DIR}/MicroMaxCmakeA10.cmake)
# cmake_minimum_required(VERSION 3.16)

# project(shared_memory)

# set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wextra -std=c++14")
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")

# include_directories(
#     ${CMAKE_CURRENT_SOURCE_DIR}/include
# )

# set(SOURCES_MASTER
#         ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
#         ${CMAKE_CURRENT_SOURCE_DIR}/src/master.cpp
#         ${CMAKE_CURRENT_SOURCE_DIR}/src/shared_memory.cpp
#         ${CMAKE_CURRENT_SOURCE_DIR}/src/shm.cpp
#         ${CMAKE_CURRENT_SOURCE_DIR}/src/pipe.cpp
#         ${CMAKE_CURRENT_SOURCE_DIR}/src/client.cpp)

# add_executable(master ${SOURCES_MASTER})