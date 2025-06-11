add_subdirectory(preprocessor)
add_subdirectory(solver)
add_subdirectory(postprocessor)

set(CMAKE_NINJA_FORCE_RESPONSE_FILE "ON" CACHE BOOL "Force Ninja to use response files.")

add_executable(fem2d main.cpp)

target_link_libraries(fem2d PUBLIC 
preprocessor
solver
postprocessor
${Boost_LIBRARIES} 
${VTK_LIBRARIES}
)
