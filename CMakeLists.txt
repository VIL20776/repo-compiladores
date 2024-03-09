cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAUKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
project(Compilador VERSION 0.1.2 LANGUAGES C CXX)

# Dependencies
find_package(Catch2 CONFIG)

find_path(VCPKG_INCLUDE_DIR graphviz PATHS ${CMAKE_CURRENT_BINARY_DIR} REQUIRED)
find_library(cgraph libcgraph NAMES cgraph REQUIRED)
find_library(gvc libgvc NAMES gvc)

# Algoritms sources
add_library(algorithms STATIC 
    src/algorithms/regex.cpp
    src/algorithms/postfix.cpp 
    # src/algorithms/constructors/thompson.cpp
    # src/algorithms/constructors/subsets.cpp
    # src/algorithms/constructors/minimize.cpp
    src/algorithms/constructors/direct.cpp
    src/algorithms/models/automaton.cpp
    src/algorithms/models/syntax_tree.cpp)
target_include_directories(algorithms PRIVATE src ${VCPKG_INCLUDE_DIR})
target_link_libraries(algorithms PRIVATE ${cgraph} ${gvc})

add_library(yalex STATIC
    src/analyzer/yalex.cpp)
target_include_directories(yalex PRIVATE src ${VCPKG_INCLUDE_DIR})
target_link_libraries(yalex PRIVATE algorithms ${cgraph} ${gvc})

# Compiler executable
add_executable(Compilador src/main.cpp)
target_include_directories(Compilador PRIVATE src ${VCPKG_INCLUDE_DIR})

target_link_libraries(Compilador PRIVATE algorithms yalex)

# Test executables
# add_executable(regex.test tests/regex.test.cpp)
# target_include_directories(regex.test PRIVATE src ${VCPKG_INCLUDE_DIR})

# target_link_libraries(regex.test PRIVATE algorithms Catch2::Catch2 Catch2::Catch2WithMain)

# include(CTest)
# enable_testing()

# include(Catch)
# catch_discover_tests(regex.test)

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)
