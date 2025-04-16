add_library(
    preprocessor 
    preprocessor.cpp 
    preprocessor.h
    tools.cpp
    tools.h
    error.cpp
    error.h
)

add_subdirectory("node")
add_subdirectory("material")
add_subdirectory("elem_service")
add_subdirectory("boundaries")
add_subdirectory("elem_service/elements")

target_include_directories(
    preprocessor PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
    ${Boost_INCLUDE_DIRS}
)
