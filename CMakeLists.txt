cmake_minimum_required(VERSION 3.28)
project(Game)

set(CMAKE_CXX_STANDARD 23)

find_package(glm CONFIG REQUIRED)
find_package(glfw3 3.4 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME} main.cpp
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
        src/Tomos/util/shader/Shader.cc
        src/Tomos/util/shader/Shader.hh
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
)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions(-DNDEBUG)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm glfw GLEW::GLEW OpenGL::GL)