# Project Boomerang (c) 2020-2021 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan

# Modified MIT License
# 
# Copyright 2020-2021 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan
# 
#  * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
#  * to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
#  * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
#  *
#  * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
#  *
#  * Restrictions:
#  >  The Software may not be sold unless significant, mechanics changing modifications are made by the seller, or unless the buyer
#  >  understands an unmodified version of the Software is available elsewhere free of charge, and agrees to buy the Software given
#  >  this knowledge.
#  *
#  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
#  * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
#  * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
#  * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

cmake_minimum_required(VERSION 3.8)

# Set project variables
set(PROJECT_VERSION "0.1.0")
set(PROJECT_DESCRIPTION "Project Boomerang is an open-source, yet-to-be-titled video game project collaboration started by four college dudes.")

# Set up Project
project(boomerang VERSION ${PROJECT_VERSION} DESCRIPTION ${PROJECT_DESCRIPTION})

# Include OpenGL
find_package(OpenGL REQUIRED)

# Set up defaults
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Uncomment this line before running if using Visual Studio's built-in CMake env.
# Set running directory to root
# set(VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/root/)          # this crap doesnt work whyyyyyyyyyyy

# Include directories
include_directories("${CMAKE_SOURCE_DIR}/includes/ASWL/root/include")
include_directories("${CMAKE_SOURCE_DIR}/includes/GLFW/include")
include_directories("${CMAKE_SOURCE_DIR}/includes/GLAD/include")
include_directories("${CMAKE_SOURCE_DIR}/includes/FREETYPE/include")

include_directories("${CMAKE_SOURCE_DIR}/includes")

add_subdirectory("includes/ASWL")
add_subdirectory("includes/GLFW")
add_subdirectory("includes/GLAD-master")
add_subdirectory("includes/GLM")
add_subdirectory("includes/FREETYPE")

# Subdirectories
add_subdirectory(root)
add_subdirectory(sandbox)
