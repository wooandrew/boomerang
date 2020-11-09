# Project Boomerang : root (c) 2020 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan

# Modified MIT License
# 
# Copyright 2020 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan
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

cmake_minimum_required (VERSION 3.8)

add_executable(main main.cpp)

add_library(
    
    # Add Engine
    BoomerangEngine STATIC

    # Main
    "engine/engine.hpp"
    "engine/engine.cpp"

    # Input
    "engine/input/mouse.hpp"                        "engine/input/mouse.cpp"
    "engine/input/keyboard.hpp"                     "engine/input/keyboard.cpp"

    # Graphics
    "engine/graphics/graphics.hpp"                  "engine/graphics/graphics.cpp"
    "engine/graphics/vertex.hpp"                    "engine/graphics/vertex.cpp"
    "engine/graphics/buffer.hpp"                    "engine/graphics/buffer.cpp"
    "engine/graphics/shaders/shaders.hpp"           "engine/graphics/shaders/shaders.cpp"
)

add_library(
    
    # Add misc
    BoomerangMisc STATIC

    # files
    "misc/utilities.hpp"            "misc/utilities.cpp"
    "misc/logger.hpp"               "misc/logger.cpp"
)
# Link engine against main
target_link_libraries(main PRIVATE BoomerangEngine)
target_link_libraries(main PRIVATE BoomerangMisc)


# Link dependencies against project executable
target_link_libraries(main PRIVATE glfw)                    # Link GLFW
target_link_libraries(main PRIVATE glad)                    # Link GLAD
# GLM is header only, no library to link                    # Link GLM
