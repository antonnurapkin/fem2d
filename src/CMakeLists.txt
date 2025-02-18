cmake_minimum_required(VERSION 3.23)

project(fem2d)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Comment 2 lines below if you don't use vckpg
set(VCPKG_TOOLCHAIN_FILE "D:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE FILEPATH "Path to vcpkg toolchain")
include(${VCPKG_TOOLCHAIN_FILE})
# find_package(VTK REQUIRED)
find_package(VTK REQUIRED PATHS "D:/vcpkg/installed/x64-windows/share/vtk" NO_DEFAULT_PATH)
find_package(Boost 1.86.0 REQUIRED)
#find_package(Boost 1.86.0 REQUIRED COMPONENTS numeric_ublas)
# find_package(VTK COMPONENTS 
#   CommonColor
#   CommonCore
#   CommonTransforms
#   FiltersSources
#   RenderingAnnotation
#   InteractionStyle
#   RenderingContextOpenGL2
#   RenderingCore
#   RenderingFreeType
#   RenderingGL2PSOpenGL2
#   RenderingOpenGL2
# )


add_subdirectory(solver)
add_subdirectory(preprocessor)
add_subdirectory(postprocessor)

set(CMAKE_NINJA_FORCE_RESPONSE_FILE "ON" CACHE BOOL "Force Ninja to use response files.")

add_executable(fem2d main.cpp)

target_link_libraries(fem2d PUBLIC solver_lib preprocessor_lib postprocessor_lib ${Boost_LIBRARIES} ${VTK_LIBRARIES})

vtk_module_autoinit(
  TARGETS fem2d
  MODULES ${VTK_LIBRARIES}
)
