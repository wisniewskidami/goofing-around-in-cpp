cmake_minimum_required(VERSION 4.1)

project(libwebsockets_example)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 20)

find_package(PkgConfig REQUIRED)

pkg_check_modules(LIBWEBSOCKETS REQUIRED libwebsockets)

file(COPY mount-origin DESTINATION ${CMAKE_BINARY_DIR})

add_executable(libwebsockets_example
        main.cpp
        LwsHttpServer.class.hpp
        LwsHttpServer.class.cpp
)

target_link_libraries(libwebsockets_example PRIVATE ${LIBWEBSOCKETS_LIBRARIES})
target_include_directories(libwebsockets_example PRIVATE ${LIBWEBSOCKETS_INCLUDE_DIRS})
target_compile_definitions(libwebsockets_example PRIVATE ${LIBWEBSOCKETS_CFLAGS_OTHER})
