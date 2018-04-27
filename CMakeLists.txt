cmake_minimum_required(VERSION 2.8)

project(inception-rtdebugger-driver)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set (CMAKE_CXX_STANDARD 11)

include_directories("include")

file(GLOB SOURCES src/benchmark/*.cpp
src/builder/*.cpp
src/builder/file_builder/*.cpp
src/builder/inception/*.cpp
src/builder/jtag/*.cpp
src/decoders/*.cpp
src/decoders/inception/*.cpp
src/decoders/jtag/*.cpp
src/drivers/*.cpp
src/drivers/flash/*.cpp
src/device/*.cpp
src/interface/*.cpp
src/test/*.cpp
src/trace/*.cpp
src/binutils/*.cpp
src/*.cpp)

add_executable(inception-db-suite ${SOURCES})

target_link_libraries(inception-db-suite pthread usb-1.0)

