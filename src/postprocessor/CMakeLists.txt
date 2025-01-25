add_library(postprocessor_lib postprocessor.cpp postprocessor.h vizualizationParams.h)

target_include_directories(
    postprocessor_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${Boost_INCLUDE_DIRS}
    ${VTK_INCLUDE_DIRS}
)

add_subdirectory("renderer")
add_subdirectory("geometry")
add_subdirectory("boundaries")

target_link_libraries(postprocessor_lib PUBLIC ${VTK_LIBRARIES})

vtk_module_autoinit(
  TARGETS postprocessor_lib
  MODULES ${VTK_LIBRARIES}
)
