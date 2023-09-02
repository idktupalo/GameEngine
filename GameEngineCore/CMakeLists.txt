cmake_minimum_required(VERSION 3.12)

set(CORE_ENGINE_NAME GameEngineCore)

set(CORE_PUBLIC_INCLUDES
    includes/Application.hpp
    includes/Log.hpp
    includes/Event.hpp
)

set(CORE_PRIVATE_INCLUDES
    src/Window.hpp
    src/Rendering/ShaderProgram.hpp
    src/Rendering/VertexBuffer.hpp
    src/Rendering/VertexArray.hpp
    src/Rendering/ElementBuffer.hpp
)

set(CORE_PRIVATE_SOURCES
    src/Window.cpp
    src/Application.cpp
    src/Rendering/ShaderProgram.cpp
    src/Rendering/VertexBuffer.cpp
    src/Rendering/VertexArray.cpp
    src/Rendering/ElementBuffer.cpp
)

set(CORE_ALL_SOURCES
    ${CORE_PUBLIC_INCLUDES}
    ${CORE_PRIVATE_INCLUDES}
    ${CORE_PRIVATE_SOURCES}
)

add_library(${CORE_ENGINE_NAME} STATIC ${CORE_ALL_SOURCES})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${CORE_ALL_SOURCES})

target_include_directories(${CORE_ENGINE_NAME} PUBLIC includes)
target_include_directories(${CORE_ENGINE_NAME} PRIVATE src)

target_compile_features(${CORE_ENGINE_NAME} PUBLIC cxx_std_17)

add_subdirectory(../External/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
add_subdirectory(../External/spdlog ${CMAKE_CURRENT_BINARY_DIR}/spdlog)
add_subdirectory(../External/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)
add_subdirectory(../External/glm ${CMAKE_CURRENT_BINARY_DIR}/glm)

target_link_libraries(${CORE_ENGINE_NAME} PRIVATE glfw)
target_link_libraries(${CORE_ENGINE_NAME} PRIVATE spdlog)
target_link_libraries(${CORE_ENGINE_NAME} PRIVATE glad)
target_link_libraries(${CORE_ENGINE_NAME} PRIVATE glm)

set(IMGUI_INCLUDES
    ../external/imgui/imgui.h
	../external/imgui/backends/imgui_impl_glfw.h
	../external/imgui/backends/imgui_impl_opengl3.h
)

set(IMGUI_SRC
    ../external/imgui/imgui.cpp
	../external/imgui/backends/imgui_impl_glfw.cpp
	../external/imgui/backends/imgui_impl_opengl3.cpp
	../external/imgui/imgui_demo.cpp
	../external/imgui/imgui_tables.cpp
	../external/imgui/imgui_widgets.cpp
	../external/imgui/imgui_draw.cpp
)

add_library(ImGui STATIC
    ${IMGUI_INCLUDES}
    ${IMGUI_SRC}
)

target_include_directories(ImGui PUBLIC ../external)
target_include_directories(ImGui PUBLIC ../external/imgui)

target_link_libraries(ImGui PRIVATE glad glfw)
target_link_libraries(${CORE_ENGINE_NAME} PRIVATE ImGui)