cmake_minimum_required(VERSION 3.0.0)

project(Compilador VERSION 0.1.0 LANGUAGES C CXX)

# include(CTest)
# enable_testing()

find_library(cgraph libcgraph NAMES cgraph )

add_library(algorithms STATIC src/algorithms/postfix.cpp)
target_include_directories(algorithms PUBLIC src/algorithms)

add_executable(Compilador src/main.cpp)
target_include_directories(Compilador PUBLIC src)

target_link_libraries(Compilador PUBLIC ${cgraph} algorithms)

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)
