add_library(postprocessor_lib postprocessor.cpp postprocessor.h)

target_include_directories(
    postprocessor_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${Boost_INCLUDE_DIRS}
    ${VTK_INCLUDE_DIRS}
)

target_link_libraries(postprocessor_lib PUBLIC ${VTK_LIBRARIES})

vtk_module_autoinit(
  TARGETS postprocessor_lib
  MODULES ${VTK_LIBRARIES}
)
