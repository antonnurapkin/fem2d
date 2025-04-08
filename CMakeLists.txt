cmake_minimum_required(VERSION 3.23)
project(fem2d)

find_package(Boost)
find_package(VTK COMPONENTS 
  CommonColor
  CommonCore
  CommonTransforms
  FiltersSources
  RenderingAnnotation
  InteractionStyle
  RenderingContextOpenGL2
  RenderingCore
  RenderingFreeType
  RenderingGL2PSOpenGL2
  RenderingOpenGL2
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Добавляем папку src как подпроект
add_subdirectory(src)

vtk_module_autoinit(
  TARGETS fem2d
  MODULES ${VTK_LIBRARIES}
)

