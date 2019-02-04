cmake_minimum_required(VERSION 3.13.1)
include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
project(toggle_led)

list(APPEND HEADERS
    "${PROJECT_BINARY_DIR}/include/generated"
    "${PROJECT_BINARY_DIR}/zephyr/include/generated"
    "${PROJECT_SOURCE_DIR}/include"
    )

list(APPEND SOURCES
    "${PROJECT_SOURCE_DIR}/src/main.cpp"
    "${PROJECT_SOURCE_DIR}/zeta-lib/src/gpio_controller.cpp"
    "${PROJECT_SOURCE_DIR}/zeta-lib/src/property.cpp"
    )

include_directories(${HEADERS})
target_sources(app PRIVATE ${SOURCES})