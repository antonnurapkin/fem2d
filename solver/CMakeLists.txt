add_library(solver_lib Solver.cpp Solver.h)

add_subdirectory(utils)
add_subdirectory(node)
add_subdirectory(material)
add_subdirectory(elem_service)
add_subdirectory(boundaries)

target_include_directories(
    solver_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${Boost_INCLUDE_DIRS}
)
