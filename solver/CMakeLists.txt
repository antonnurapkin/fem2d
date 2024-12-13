cmake_minimum_required(VESRION 3.10)

aux_source_directory(. SRC_FILES)

add_library(solver_lib ${SRC_FILES})

target_include_directories(
    solver_lib PUBLIC_HEADER
    ${Boost_INCLUDE_DIRS}
)   

