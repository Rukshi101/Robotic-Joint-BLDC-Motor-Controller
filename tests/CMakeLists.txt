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

target_link_options(test_velocity PRIVATE -Wl,-subsystem,console)

# Define missing extern variables for test build
target_compile_definitions(test_velocity PRIVATE
    -DTEST_BUILD
)

# Link as console app on Windows
target_link_options(test_velocity PRIVATE -mconsole)