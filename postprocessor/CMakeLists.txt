add_library(postprocessor_lib postprocessor.cpp postprocessor.h)

target_include_directories(
    postprocessor_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${Boost_INCLUDE_DIRS}
    ${VTK_INCLUDE_DIRS}
)
