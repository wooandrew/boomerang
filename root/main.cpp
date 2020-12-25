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

#include <GLM/glm/gtc/matrix_transform.hpp>

#include "engine/engine.hpp"
#include "engine/manager.hpp"
#include "engine/input/mouse.hpp"
#include "engine/input/keyboard.hpp"
#include "engine/graphics/manager.hpp"
#include "engine/graphics/shaders.hpp"
#include "engine/graphics/renderer.hpp"
#include "engine/graphics/font.hpp"
#include "engine/graphics/camera/orthocam.hpp"
#include "engine/physics/rigidbody.hpp"
#include "engine/physics/collision.hpp"

#include "misc/logger.hpp"
#include "misc/utilities.hpp"

// Move to render manager
namespace RENDER_LAYER {

    constexpr float LAYER0 = 0.0f;
    constexpr float LAYER1 = 0.1f;
    constexpr float LAYER2 = 0.2f;
    constexpr float LAYER3 = 0.3f;
}

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
    Boomerang::Core::Manager manager;

    Boomerang::Core::Input::Mouse::init();
    Boomerang::Core::Graphics::Manager::init();
    Boomerang::Core::Graphics::Renderer::init();

    //Boomerang::Core::Graphics::Manager::SetClearColor({ 0.f, 0.f, 1.f, 1.f });
    
    // Initialize Primary Orthographic Camera
    std::shared_ptr<Boomerang::Core::Graphics::OrthoCam> __camera_1 = std::make_shared<Boomerang::Core::Graphics::OrthoCam>(
                                                                      glm::ortho(-engine.GetWindowDimensions().x / 2.f, engine.GetWindowDimensions().x / 2.f,
                                                                                 -engine.GetWindowDimensions().y / 2.f, engine.GetWindowDimensions().y / 2.f), 500.f);

    std::shared_ptr<Boomerang::Core::Graphics::OrthoCam> __camera_f = std::make_shared<Boomerang::Core::Graphics::OrthoCam>(
                                                                      glm::ortho(-engine.GetWindowDimensions().x / 2.f, engine.GetWindowDimensions().x / 2.f,
                                                                                 -engine.GetWindowDimensions().y / 2.f, engine.GetWindowDimensions().y / 2.f), 500.f);

    __camera_1->SetLock(true);
    __camera_f->SetLock(true);

    std::shared_ptr<Boomerang::Core::Graphics::Texture> demo = std::make_shared<Boomerang::Core::Graphics::Texture>("assets/projectboomerang.png");
    std::shared_ptr<Boomerang::Core::Graphics::Font> font = std::make_shared<Boomerang::Core::Graphics::Font>();
    font->init("raleway", "assets/fonts/raleway.ttf", 24);

    Boomerang::Core::Physics::Rigidbody r1({ 0, 0, RENDER_LAYER::LAYER1 }, { 50, 50 });
    Boomerang::Core::Physics::Rigidbody r2({ 100, 100, RENDER_LAYER::LAYER2 }, { 50, 50 });


    while (manager.run(engine.GetWindow())) {

        engine.update();
        manager.update();

        __camera_1->update(manager.dt());

        // ***** TEST ***** \\

        glm::vec3 position = r2.GetPosition();
        static float rotation = 0;

        if (Boomerang::Core::Input::Keyboard::KeyIsPressed(GLFW_KEY_W)) {               // UP
            position.x -= std::sin(glm::radians(0.f)) * 300 * manager.dt();
            position.y += std::cos(glm::radians(0.f)) * 300 * manager.dt();
        }
        else if (Boomerang::Core::Input::Keyboard::KeyIsPressed(GLFW_KEY_S)) {          // DOWN
            position.x += std::sin(glm::radians(0.f)) * 300 * manager.dt();
            position.y -= std::cos(glm::radians(0.f)) * 300 * manager.dt();
        }

        if (Boomerang::Core::Input::Keyboard::KeyIsPressed(GLFW_KEY_A)) {               // LEFT
            position.x -= std::cos(glm::radians(0.f)) * 300 * manager.dt();
            position.y -= std::sin(glm::radians(0.f)) * 300 * manager.dt();
        }
        else if (Boomerang::Core::Input::Keyboard::KeyIsPressed(GLFW_KEY_D)) {          // RIGHT
            position.x += std::cos(glm::radians(0.f)) * 300 * manager.dt();
            position.y += std::sin(glm::radians(0.f)) * 300 * manager.dt();
        }

        if (Boomerang::Core::Input::Keyboard::KeyIsPressed(GLFW_KEY_Q))                 // ROTATE LEFT
            rotation += 100 * manager.dt();
        else if (Boomerang::Core::Input::Keyboard::KeyIsPressed(GLFW_KEY_E))            // ROTATE RIGHT
            rotation -= 100 * manager.dt();

        r2.update(position, rotation);
        // ***** TEST ***** \\

        glm::vec4 color = { 0, 1.f, 0, 1.f };
        if(Boomerang::Core::Physics::Collision::SAT(r1, r2))
            color = { 1.f, 0, 0, 1.f };

        Boomerang::Core::Graphics::Manager::BeginRender();

        Boomerang::Core::Graphics::Renderer::StartScene(__camera_1);
        Boomerang::Core::Graphics::Renderer::RenderTexture({ 0, 0, RENDER_LAYER::LAYER0 }, { 1.f, 1.f }, demo);
        Boomerang::Core::Graphics::Renderer::DrawQuad(r1.GetPosition(), r1.GetSize(), { 1.f, 1.f, 1.f, 1.f });
        Boomerang::Core::Graphics::Renderer::DrawQuad(r2.GetPosition(), r2.GetSize(), rotation, color);

        Boomerang::Core::Graphics::Renderer::DrawQuad({ r2.UpperLeftVertex.x, r2.UpperLeftVertex.y, RENDER_LAYER::LAYER3 }, { 10, 10 }, {0.f, 1.f, 1.f, 1.f});
        Boomerang::Core::Graphics::Renderer::DrawQuad({ r2.UpperRightVertex.x, r2.UpperRightVertex.y, RENDER_LAYER::LAYER3 }, { 10, 10 }, { 1.f, 0.f, 1.f, 1.f });
        Boomerang::Core::Graphics::Renderer::DrawQuad({ r2.LowerLeftVertex.x, r2.LowerLeftVertex.y, RENDER_LAYER::LAYER3 }, { 10, 10 }, { 1.f, 0.f, 0.f, 1.f });
        Boomerang::Core::Graphics::Renderer::DrawQuad({ r2.LowerRightVertex.x, r2.LowerRightVertex.y, RENDER_LAYER::LAYER3 }, { 10, 10 }, { 0.f, 0.f, 1.f, 1.f });
        Boomerang::Core::Graphics::Renderer::EndScene();

        Boomerang::Core::Graphics::Renderer::StartScene(__camera_f);
        Boomerang::Core::Graphics::Renderer::RenderText("Boomerang", { -495, 295, RENDER_LAYER::LAYER1 }, { 1.f, 1.f }, { 1.f, 1.f, 1.f }, font);
        Boomerang::Core::Graphics::Renderer::EndScene();

        Boomerang::Core::Graphics::Manager::EndRender(engine.GetWindow());
    }

    Boomerang::Core::Graphics::Renderer::shutdown();
    Boomerang::Core::Graphics::Manager::shutdown();

    return 0;
}
