cmake_minimum_required(VERSION 3.20)
project(algolib CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

option(ALGOLIB_SANITIZE "Compilar tests con AddressSanitizer + UBSan" ON)

add_library(algolib INTERFACE)
target_include_directories(algolib INTERFACE include)

enable_testing()

# Un ejecutable por lab: lab01_tests, lab02_tests, ...
#   cmake --build build --target lab03_tests && ./build/lab03_tests
#   ctest --test-dir build -R lab03
file(GLOB LAB_DIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/tests ${CMAKE_CURRENT_SOURCE_DIR}/tests/lab*)
foreach(lab ${LAB_DIRS})
    file(GLOB srcs CONFIGURE_DEPENDS tests/${lab}/*.cpp)
    if(NOT srcs)
        continue()
    endif()
    add_executable(${lab}_tests ${srcs} tests/common/algotest_main.cpp)
    target_include_directories(${lab}_tests PRIVATE tests/common)
    target_link_libraries(${lab}_tests PRIVATE algolib)
    target_compile_definitions(${lab}_tests PRIVATE ALGOTEST_SUITE="${lab}")
    target_compile_options(${lab}_tests PRIVATE -Wall -Wextra -Wpedantic)
    if(ALGOLIB_SANITIZE)
        target_compile_options(${lab}_tests PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
        target_link_options(${lab}_tests PRIVATE -fsanitize=address,undefined)
    endif()
    add_test(NAME ${lab} COMMAND ${lab}_tests)
endforeach()
