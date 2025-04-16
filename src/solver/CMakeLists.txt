add_library(solver solver.cpp solver.h tools.h tools.cpp)

target_include_directories(
    solver PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    PRIVATE
    ${Boost_INCLUDE_DIRS}
)

target_link_libraries(solver PRIVATE preprocessor)