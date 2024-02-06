cmake_minimum_required(VERSION 3.0.0)

# set(CXX_STANDARD 20)
# set(CXX_STANDARD_REQUIRED ON)
project(Compilador VERSION 0.1.0 LANGUAGES C CXX)

# include(CTest)
# enable_testing()

find_library(cgraph libcgraph NAMES cgraph)
find_library(gvc libgbc NAMES gvc)

# add_library(models STATIC
#     src/algorithms/models/automaton.cpp)
# target_include_directories(models PRIVATE src/algorithms/models)

add_library(algorithms STATIC 
    src/algorithms/postfix.cpp 
    src/algorithms/thompson.cpp
    src/algorithms/models/automaton.cpp)
target_include_directories(algorithms PRIVATE src/algorithms)

# target_link_libraries(algorithms PUBLIC models)

add_executable(Compilador src/main.cpp)
target_include_directories(Compilador PUBLIC src)

target_link_libraries(Compilador PUBLIC ${cgraph} ${gvc} algorithms)

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)
