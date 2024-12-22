add_library(preprocessor_lib preprocessor.cpp preprocessor.h)

add_subdirectory("utils")
add_subdirectory("node")
add_subdirectory("material")
add_subdirectory("elem_service")
add_subdirectory("boundaries")
add_subdirectory("elem_service/elements")

target_include_directories(
    preprocessor_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${Boost_INCLUDE_DIRS}
)