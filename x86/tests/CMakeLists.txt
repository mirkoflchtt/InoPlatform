cmake_minimum_required(VERSION 2.8)
project(InoPlatformTests)

set(TESTS_DEPS
    InoPlatform
    m)

add_executable(test_ino_rbuffer test_ino_rbuffer.c)
target_link_libraries(test_ino_rbuffer ${TESTS_DEPS})
install_bin(test_ino_rbuffer)

add_executable(test_ino_dlist test_ino_dlist.c)
target_link_libraries(test_ino_dlist ${TESTS_DEPS})
install_bin(test_ino_dlist)

add_executable(test_ino_event_handler test_ino_event_handler.cpp)
target_link_libraries(test_ino_event_handler ${TESTS_DEPS})
install_bin(test_ino_event_handler)
