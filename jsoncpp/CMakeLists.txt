cmake_minimum_required(VERSION 4.1)

project(jsoncpp_example)

set(CMAKE_CXX_STANDARD 20)

find_package(PkgConfig REQUIRED)

pkg_check_modules(JSONCPP REQUIRED jsoncpp)
pkg_check_modules(CRITERION REQUIRED criterion)

configure_file(test.json test.json COPYONLY)

add_executable(jsoncpp_example
        io.hpp
        main.test.cpp
        test.json
)

target_link_libraries(jsoncpp_example PRIVATE ${JSONCPP_LIBRARIES} ${CRITERION_LIBRARIES})
target_include_directories(jsoncpp_example PRIVATE ${JSONCPP_INCLUDE_DIRS} ${CRITERION_INCLUDE_DIRS})
target_compile_options(jsoncpp_example PRIVATE ${JSONCPP_CFLAGS_OTHER} ${CRIERION_CFLAGS_OTHER})

