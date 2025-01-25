add_library(solver_lib solver.cpp solver.h)

add_subdirectory("utils")

target_include_directories(
    solver_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${Boost_INCLUDE_DIRS}
)
