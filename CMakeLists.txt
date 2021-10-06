cmake_minimum_required(VERSION 3.10)

project(Test)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wextra")

add_executable(serv timeserv.c)