#TODO: Неверно работает CMake
aux_source_directory(. SRC_FILES)

target_sources(preprocessor_lib PUBLIC ${SRC_FILES})