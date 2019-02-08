cmake_minimum_required(VERSION 3.13.1)
# set(BOARD nrf52_pca10040)
set(BOARD_ROOT ${CMAKE_CURRENT_LIST_DIR})
# set(BOARD tc_bait_p1)
set(BOARD nucleo_f091rc)

include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
project(toggle_led)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")


list(APPEND HEADERS
    "${PROJECT_BINARY_DIR}/include/generated"
    "${PROJECT_BINARY_DIR}/zephyr/include/generated"
    "${PROJECT_SOURCE_DIR}/include"
    "${PROJECT_SOURCE_DIR}/../../zeta/"
    )

list(APPEND SOURCES
    "${PROJECT_SOURCE_DIR}/src/main.cpp"
    # "${PROJECT_SOURCE_DIR}/src/main.c"
    "${PROJECT_SOURCE_DIR}/../../zeta/zeta.cpp"
    )


include_directories(${HEADERS})
target_sources(app PRIVATE ${SOURCES})

include(${PROJECT_SOURCE_DIR}/mcumgr.cmake NO_POLICY_SCOPE)
# target_include_directories(app PRIVATE $ENV{ZEPHYR_BASE}/subsys/fs/nvs)