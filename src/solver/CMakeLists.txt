add_library(solver solver.cpp solver.h)

add_subdirectory("utils")

target_include_directories(
    solver PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${Boost_INCLUDE_DIRS}
)
