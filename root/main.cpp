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

#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>

#include <iostream>

#include <GLM/glm/gtc/matrix_transform.hpp>

#include "engine/engine.hpp"
#include "engine//input/mouse.hpp"
#include "engine/graphics/manager.hpp"
#include "engine/graphics/shaders.hpp"
#include "engine/graphics/renderer.hpp"
#include "engine/graphics/camera/orthocam.hpp"
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

    // Initialize Engine components
    Boomerang::Core::Input::Mouse::init();
    Boomerang::Core::Graphics::Manager::init();
    Boomerang::Core::Graphics::Renderer::init();

    //Boomerang::Core::Graphics::Manager::SetClearColor({ 1.f, 1.f, 1.f, 1.f });
    
    // Initialize Primary Orthographic Camera
    Boomerang::Core::Graphics::OrthoCam __camera_1(glm::ortho(-engine.GetWindowDimensions().x / 2.f, engine.GetWindowDimensions().x / 2.f,
                                                              -engine.GetWindowDimensions().y / 2.f, engine.GetWindowDimensions().y / 2.f), 500.f);

    std::shared_ptr<Boomerang::Core::Graphics::Texture> demo = std::make_shared<Boomerang::Core::Graphics::Texture>("assets/demo.png");

    GAME_STATE state = GAME_STATE::RUN;

    while (!glfwWindowShouldClose(engine.GetWindow()) && state == GAME_STATE::RUN) {

        engine.Update();

        Boomerang::Core::Graphics::Manager::BeginRender();

        Boomerang::Core::Graphics::Renderer::StartScene(__camera_1);
        //Boomerang::Core::Graphics::Renderer::DrawQuad({ 0, 0 }, { 100, 100 }, { 1.f, 1.f, 1.f, 1.f });
        Boomerang::Core::Graphics::Renderer::RenderTexture({ 0, 0 }, { 1.f, 1.f }, demo);
        Boomerang::Core::Graphics::Renderer::EndScene();

        Boomerang::Core::Graphics::Manager::EndRender(engine.GetWindow());
    }

    Boomerang::Core::Graphics::Renderer::shutdown();
    Boomerang::Core::Graphics::Manager::shutdown();

    return 0;
}
