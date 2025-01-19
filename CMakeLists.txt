cmake_minimum_required(VERSION 3.30)
project(Game)

set(CMAKE_CXX_STANDARD 23)

find_package(glm CONFIG REQUIRED)
find_package(glfw3 3.4 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME} main.cpp
        src/Tomos/Util/Logger/Logger.cc
        src/Tomos/Util/Logger/Logger.hh
        src/Tomos/Core/Window.cc
        src/Tomos/Core/Window.hh
        src/Tomos/Events/Event.cc
        src/Tomos/Events/Event.hh
        src/Tomos/Events/Key/KeyEvent.cc
        src/Tomos/Events/Key/KeyEvent.hh
        src/Tomos/Events/Mouse/MouseEvent.cc
        src/Tomos/Events/Mouse/MouseEvent.hh
        src/Tomos/Events/Application/ApplicationEvent.cc
        src/Tomos/Events/Application/ApplicationEvent.hh
        src/Tomos/Core/Application.cc
        src/Tomos/Core/Application.hh
        src/Tomos/Util/Shader/Shader.cc
        src/Tomos/Util/Shader/Shader.hh
        src/Tomos/Core/Node.cc
        src/Tomos/Core/Node.hh
)

target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm glfw GLEW::GLEW OpenGL::GL)