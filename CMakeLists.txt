cmake_minimum_required(VERSION 3.0.0)

# set(CXX_STANDARD 20)
# set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
project(Compilador VERSION 0.1.0 LANGUAGES C CXX)

# include(CTest)
# enable_testing()

# find_package(libcgraph CONFIG)
find_package(Catch2 CONFIG)

find_library(cgraph libcgraph NAMES cgraph)
find_library(gvc libgvc NAMES gvc)

find_path(GRAPHVIZ_INCLUDE_DIR graphviz PATHS ${CMAKE_CURRENT_BINARY_DIR} REQUIRED)

add_library(algorithms STATIC 
    src/algorithms/regex.cpp
    src/algorithms/postfix.cpp 
    src/algorithms/constructors/thompson.cpp
    src/algorithms/constructors/subsets.cpp
    src/algorithms/constructors/minimize.cpp
    src/algorithms/constructors/direct.cpp
    src/algorithms/models/automaton.cpp
    src/algorithms/models/syntax_tree.cpp)
target_include_directories(algorithms PRIVATE src ${GRAPHVIZ_INCLUDE_DIR})
target_link_libraries(algorithms PRIVATE ${cgraph} ${gvc})

# target_link_libraries(algorithms PUBLIC models)

add_executable(Compilador src/main.cpp)
target_include_directories(Compilador PRIVATE src ${GRAPHVIZ_INCLUDE_DIR})

target_link_libraries(Compilador PRIVATE algorithms)

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)
