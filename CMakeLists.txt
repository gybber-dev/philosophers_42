cmake_minimum_required(VERSION 3.20)
project(philosophers C)
set(CMAKE_C_STANDARD 99)

FILE(GLOB PHILO_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/*.c)


FILE(GLOB SANDBOX_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/sandbox/*.c)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

#list(FILTER TEST_SOURCES EXCLUDE REGEX "${CMAKE_CURRENT_SOURCE_DIR}/src/main.c")
add_executable(philosophers
        ${PHILO_SOURCES})

add_executable(sandbox
        ${SANDBOX_SOURCES})

target_compile_options(
        philosophers PRIVATE
        #        -Wall -Wextra -Werror
)