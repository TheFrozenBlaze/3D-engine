cmake_minimum_required(VERSION 3.16)
project(Radon VERSION 0.1 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Radon
	src/glad.c
	src/main.cpp
)

target_include_directories(Radon PRIVATE 
	${CMAKE_SOURCE_DIR}/include
)

target_compile_definitions(Radon PRIVATE GLFW_STATIC)

target_link_libraries(Radon PRIVATE
	${CMAKE_SOURCE_DIR}/lib/libglfw3.a
	opengl32
)
	
	