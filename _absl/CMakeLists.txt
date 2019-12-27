cmake_minimum_required(VERSION 3.5)

project(my_project)

# Abseil requires C++11
set(CMAKE_CXX_STANDARD 11)

# Process Abseil's CMake build system
add_subdirectory(abseil-cpp)

add_executable(run-test test.cpp)

# Declare dependency on the absl::strings library
find_package(Threads)
target_link_libraries(run-test absl::container absl::raw_hash_set absl::hash absl::hashtablez_sampler Threads::Threads)
