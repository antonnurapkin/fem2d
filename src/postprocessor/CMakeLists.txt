add_library(postprocessor postprocessor.cpp postprocessor.h vizualizationParams.h)

target_include_directories(
    postprocessor PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${Boost_INCLUDE_DIRS}
    ${VTK_INCLUDE_DIRS}
)

add_subdirectory("renderer")
add_subdirectory("geometry")
add_subdirectory("boundaries")

target_link_libraries(postprocessor PUBLIC ${VTK_LIBRARIES})