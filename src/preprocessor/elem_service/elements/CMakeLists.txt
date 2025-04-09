#TODO: Неверно работает CMake
aux_source_directory(. SRC_FILES)

target_sources(preprocessor PUBLIC ${SRC_FILES})