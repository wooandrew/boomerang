// Project Boomerang : main.cpp (c) 2020-2021 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan

/* Modified MIT License
 *
 * Copyright 2020-2021 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan
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
#include <thread>

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

#include "engine/world/world.hpp"
#include "engine/world/grid.hpp"
#include "engine/world/chunk.hpp"

#include <ASWL/logger.hpp>
#include <ASWL/utilities.hpp>

// TODO: Move to render manager
namespace RENDER_LAYER {

    constexpr float LAYER0 = 0.0f;
    constexpr float LAYER1 = 0.1f;
    constexpr float LAYER2 = 0.2f;
    constexpr float LAYER3 = 0.3f;
    constexpr float LAYER4 = 0.4f;
}

int main() {

    ASWL::Logger::logger("     ", "Hello, Project Boomerang!");

    Boomerang::Core::Engine engine;
    
    if (engine.init() != 0) {
        ASWL::Logger::logger("  E  ", "Fatal Error: Failed to initialize game engine.");
        return -1;
    }
    else
        ASWL::Logger::logger("  E  ", "Engine initialization success. All systems go!");

    // Initialize Engine components
    Boomerang::Core::Manager manager;

    Boomerang::Core::Input::Mouse::init();
    Boomerang::Core::Graphics::Manager::init();
    Boomerang::Core::Graphics::Renderer::init();

    //Boomerang::Core::Graphics::Manager::SetClearColor({ 1.f, 1.f, 1.f, 1.f });
    
    glm::mat4 ortho = glm::ortho(-engine.GetWindowDimensions().x / 2.f, engine.GetWindowDimensions().x / 2.f,
                                 -engine.GetWindowDimensions().y / 2.f, engine.GetWindowDimensions().y / 2.f);

    // Initialize Primary Orthographic Camera
    std::shared_ptr<Boomerang::Core::Graphics::OrthoCam> __camera_1 = std::make_shared<Boomerang::Core::Graphics::OrthoCam>(ortho, 500.f);
    std::shared_ptr<Boomerang::Core::Graphics::OrthoCam> __camera_f = std::make_shared<Boomerang::Core::Graphics::OrthoCam>(ortho, 500.f);
    std::shared_ptr<Boomerang::Core::Graphics::OrthoCam> __camera_g = std::make_shared<Boomerang::Core::Graphics::OrthoCam>(ortho, 500.f);

    //__camera_1->SetLock(true);
    __camera_f->SetLock(true);

    std::shared_ptr<Boomerang::Core::Graphics::Texture> demo = std::make_shared<Boomerang::Core::Graphics::Texture>("assets/projectboomerang.png");
    std::shared_ptr<Boomerang::Core::Graphics::Font> nsjpl_56 = std::make_shared<Boomerang::Core::Graphics::Font>();
    nsjpl_56->init("nsjpl_56", "assets/fonts/nsjpl.otf", 56);

    std::shared_ptr<Boomerang::Core::Graphics::Font> nsjpl_32 = std::make_shared<Boomerang::Core::Graphics::Font>();
    nsjpl_32->init("nsjpl_32", "assets/fonts/nsjpl.otf", 32);

    Boomerang::Core::World::Grid WorldGrid;
    WorldGrid.init({ 0, 0, 0 }, engine.GetWindowDimensions());

    glm::vec3 position = { 0, 0, 0 };

    ASWL::Utilities::FramesPerSecond::UpdateFPS();

    while (manager.run(engine.GetWindow())) {

        if (Boomerang::Core::Input::Keyboard::KeyIsPressed(GLFW_KEY_Q))                 // QUIT
            manager.state = Boomerang::Core::Manager::GAME_STATE::STOP;

        ASWL::Utilities::FramesPerSecond::UpdateFPS();
        double fps = ASWL::Utilities::FramesPerSecond::GetFPS();

        engine.update();
        manager.update();

        __camera_1->update(manager.dt());

        if (Boomerang::Core::Input::Keyboard::KeyIsPressed(GLFW_KEY_W))                 // UP
            position.y += 300 * manager.dt();
        else if (Boomerang::Core::Input::Keyboard::KeyIsPressed(GLFW_KEY_S))            // DOWN
            position.y -= 300 * manager.dt();
        
        if (Boomerang::Core::Input::Keyboard::KeyIsPressed(GLFW_KEY_A))                 // LEFT
            position.x -= 300 * manager.dt();
        else if (Boomerang::Core::Input::Keyboard::KeyIsPressed(GLFW_KEY_D))            // RIGHT
            position.x += 300 * manager.dt();

        Boomerang::Core::Graphics::Manager::BeginRender();

        int chunks_rendered = 0;

        __camera_1->SetPosition(position);
        Boomerang::Core::Graphics::Renderer::StartScene(__camera_1);
        for (auto const& [key, chunk] : WorldGrid.GetMap()) {

            if (chunk->InFrame(__camera_1->GetPosition(), engine.GetWindowDimensions())) {
                Boomerang::Core::Graphics::Renderer::RenderChunk(chunk, WorldGrid.GetCellSize(), engine.GetWindowDimensions(), position);
                chunks_rendered++;
            }
        }

        Boomerang::Core::Graphics::Renderer::EndScene();

        Boomerang::Core::Graphics::Renderer::StartScene(__camera_g, "grid");
        Boomerang::Core::Graphics::Renderer::RenderGrid(engine.GetWindowDimensions(), __camera_1->GetPosition(), WorldGrid.GetCellSize());
        Boomerang::Core::Graphics::Renderer::EndScene();

        Boomerang::Core::Graphics::Renderer::StartScene(__camera_f, "text");
        Boomerang::Core::Graphics::Renderer::RenderText("Boomerang 1nv0.1.0-pre.3-alpha", { -950, 500, RENDER_LAYER::LAYER1 }, { 1.f, 1.f }, glm::vec3(1.f), nsjpl_56);
        Boomerang::Core::Graphics::Renderer::RenderText(std::to_string((int)fps), { 900, 520, RENDER_LAYER::LAYER1 }, { 1.f, 1.f }, glm::vec3(0, 1, 0), nsjpl_32);
        Boomerang::Core::Graphics::Renderer::RenderText("Chunks Rendered: " + std::to_string((int)chunks_rendered), { -130, 0, RENDER_LAYER::LAYER1 }, { 1.f, 1.f }, glm::vec3(0, 1, 0), nsjpl_32);
        Boomerang::Core::Graphics::Renderer::EndScene();

        Boomerang::Core::Graphics::Manager::EndRender(engine.GetWindow());
    }

    Boomerang::Core::Graphics::Renderer::shutdown();
    Boomerang::Core::Graphics::Manager::shutdown();

    return 0;
}
