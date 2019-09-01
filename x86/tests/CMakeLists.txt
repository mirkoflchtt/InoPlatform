cmake_minimum_required(VERSION 2.8)
project(InoPlatformTests)

set(TESTS_DEPS
    InoPlatform
    m)
add_executable(test_rbuffer test_rbuffer.c)
target_link_libraries(test_rbuffer ${TESTS_DEPS})

install_bin(test_rbuffer)
