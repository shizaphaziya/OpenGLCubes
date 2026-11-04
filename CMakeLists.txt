cmake_minimum_required(VERSION 3.16)
project(OpenGLCubes LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

add_executable(opengl-cubes
    main.cpp
    Application.cpp
    Physics.cpp
    Renderer.cpp
)

target_link_libraries(opengl-cubes PRIVATE OpenGL::GL OpenGL::GLU GLUT::GLUT)
