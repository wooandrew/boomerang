// Project Boomerang : main.cpp (c) 2020 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan

/* Modified MIT License
 *
 * Copyright 2020 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * Restrictions:
 >  The Software may not be sold unless significant, mechanics changing modifications are made by the seller, or unless the buyer
 >  understands an unmodified version of the Software is available elsewhere free of charge, and agrees to buy the Software given
 >  this knowledge.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


#include <iostream>

#include "engine/engine.hpp"
#include "engine/graphics/manager.hpp"
#include "engine/graphics/shaders/shaders.hpp"
#include "misc/logger.hpp"

// Move to game manager
enum class GAME_STATE {
    RUN,
    STOP
};

int main() {

    logger::logger("     ", "Hello, Project Boomerang!");

    Boomerang::Core::Engine engine;
    
    if (engine.init() != 0) {
        logger::logger("  E  ", "Fatal Error: Failed to initialize game engine.");
        return -1;
    }
    else
        logger::logger("  E  ", "Engine initialization success. All systems go!");

    // Initialize graphics renderer
    Boomerang::Core::Graphics::Manager::init({ 155, 255, 0, 0 });

    Boomerang::Core::Graphics::Shader BasicShader("assets/shaders/basic-vert.glsl", "assets/shaders/basic-frag.glsl");


    GAME_STATE state = GAME_STATE::RUN;

    while (!glfwWindowShouldClose(engine.GetWindow()) && state == GAME_STATE::RUN) {

        engine.Update();

        Boomerang::Core::Graphics::Manager::BeginRender();

        Boomerang::Core::Graphics::Manager::EndRender(engine.GetWindow());
    }

    return 0;
}
