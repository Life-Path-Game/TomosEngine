cmake_minimum_required(VERSION 3.28)
project(Tomos)

set(CMAKE_CXX_STANDARD 23)

find_package(glm 1.0 CONFIG REQUIRED)
find_package(glfw3 3.4 REQUIRED)
find_package(GLEW 2.1 REQUIRED)
find_package(OpenGL REQUIRED)

# Gather all .cc files recursively
file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc
)

# Gather all header directories for include paths
file(GLOB_RECURSE ENGINE_HEADERS CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hh
)
# Gather all resources
set(RESOURCE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/Tomos/resource)
set(RESOURCE_DEST_DIR ${CMAKE_BINARY_DIR}/resource)

add_library(${PROJECT_NAME} STATIC
        src/Tomos/util/logger/Logger.cc
        src/Tomos/util/logger/Logger.hh
        src/Tomos/core/Window.cc
        src/Tomos/core/Window.hh
        src/Tomos/events/Event.cc
        src/Tomos/events/Event.hh
        src/Tomos/events/key/KeyEvent.cc
        src/Tomos/events/key/KeyEvent.hh
        src/Tomos/events/mouse/MouseEvent.cc
        src/Tomos/events/mouse/MouseEvent.hh
        src/Tomos/events/application/ApplicationEvent.cc
        src/Tomos/events/application/ApplicationEvent.hh
        src/Tomos/core/Application.cc
        src/Tomos/core/Application.hh
        src/Tomos/util/renderer/Shader.cc
        src/Tomos/util/renderer/Shader.hh
        src/Tomos/core/Node.cc
        src/Tomos/core/Node.hh
        src/Tomos/lib/stb_image/stb_image.cc
        src/Tomos/lib/stb_image/stb_image.h
        src/Tomos/core/Ecs.cc
        src/Tomos/core/Ecs.hh
        src/Tomos/systems/System.hh
        src/Tomos/systems/Component.hh
        src/Tomos/util/transform/Transform.cc
        src/Tomos/util/transform/Transform.hh
        src/Tomos/systems/camera/CameraComponent.cc
        src/Tomos/systems/camera/CameraComponent.hh
        src/Tomos/systems/camera/CameraSystem.cc
        src/Tomos/systems/camera/CameraSystem.hh
        src/Tomos/util/input/Input.cc
        src/Tomos/util/input/Input.hh
        src/Tomos/core/Layer.cc
        src/Tomos/core/Layer.hh
        src/Tomos/core/Scene.cc
        src/Tomos/core/Scene.hh
        src/Tomos/util/renderer/Buffer.cc
        src/Tomos/util/renderer/Buffer.hh
        src/Tomos/util/renderer/VertexArray.cc
        src/Tomos/util/renderer/VertexArray.hh
        src/Tomos/util/renderer/Renderer.cc
        src/Tomos/util/renderer/Renderer.hh
        src/Tomos/util/time/Time.cc
        src/Tomos/util/time/Time.hh
)

# Copy shaders to build directory
if(EXISTS ${RESOURCE_SOURCE_DIR})
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${RESOURCE_DEST_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${RESOURCE_SOURCE_DIR} ${RESOURCE_DEST_DIR}
    COMMENT "Copying resources to output directory..."
)
endif()

# Automatically include all directories with headers
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions(-DNDEBUG)
endif ()

target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm glfw GLEW::GLEW OpenGL::GL)
