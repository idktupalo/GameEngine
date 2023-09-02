cmake_minimum_required(VERSION 3.12)

set(EDITOR_ENGINE_NAME GameEngineEditor)

add_executable(${EDITOR_ENGINE_NAME} src/main.cpp)

target_link_libraries(${EDITOR_ENGINE_NAME} GameEngineCore)
target_compile_features(${EDITOR_ENGINE_NAME} PUBLIC cxx_std_17)

set_target_properties(${EDITOR_ENGINE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)