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

# Проверка наличия clang-format
find_program(CLANG_FORMAT_EXECUTABLE NAMES clang-format)

if(NOT CLANG_FORMAT_EXECUTABLE)
    message(WARNING "clang-format не найден. Цель 'format' недоступна.")
else()
    # Создание цели для форматирования
    add_custom_target(format
        COMMAND ${CMAKE_COMMAND} -E echo "Форматирование кода с помощью clang-format..."
        COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_SOURCE_DIR}
            bash -c "find . -type f \\( -name '*.cpp' -o -name '*.h' -o -name '*.hpp' \\) -exec ${CLANG_FORMAT_EXECUTABLE} -i {} +"
        COMMENT "Форматирование завершено."
        VERBATIM
    )
endif()

