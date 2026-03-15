cmake_minimum_required(VERSION 3.20)
project(bldc_tests C)
set(CMAKE_C_STANDARD 11)

set(UNITY_SOURCES unity/src/unity.c)

set(APP_SOURCES ../App/Src/velocity_control.c)

include_directories(
    unity/src
    ../App/Inc
    stubs
)

add_executable(test_velocity
    test_velocity_control.c
    stubs/test_globals.c
    ${APP_SOURCES}
    ${UNITY_SOURCES}
)