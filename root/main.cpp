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
#include "engine/math/math.hpp"

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

int main(int argc, char* argv[]) {

    //ASWL::eXperimental::HandleArgs(argc, argv);
    ASWL::Logger::logger("     ", "Hello, Project Boomerang!");

    // Initialize Engine components
    Boomerang::Core::Manager manager;
    
    int ret = manager.init();
    if (ret != 0) {
        ASWL::Logger::logger("  E  ", "FATAL ERROR: Failed to initialize game manager.");
        return ret;
    }

    while (manager.run()) {

        manager.update();

        Boomerang::Core::Graphics::Manager::BeginRender();

        int chunks_rendered = 0;

        Boomerang::Core::Graphics::Renderer::StartScene(manager.GetCamera("main_0"));
        for (auto const& [key, chunk] : manager.GetWorld()->GetMap()) {

            if (chunk->InFrame(manager.GetCamera("main_0")->GetPosition(), manager.GetWindowDimensions())) {
                Boomerang::Core::Graphics::Renderer::RenderChunk(chunk, manager.GetWindowDimensions(), manager.GetCamera("main_0")->GetPosition(), manager.GetWorld()->GetCellSize());
                chunks_rendered++;
            }
        }

        Boomerang::Core::Graphics::Renderer::EndScene();

        Boomerang::Core::Graphics::Renderer::StartScene(manager.GetCamera("grid_0"), "grid");
        Boomerang::Core::Graphics::Renderer::RenderGrid(manager.GetWindowDimensions(), manager.GetCamera("main_0")->GetPosition(), manager.GetWorld()->GetCellSize());
        Boomerang::Core::Graphics::Renderer::EndScene();

        Boomerang::Core::Graphics::Renderer::StartScene(manager.GetCamera("text_0"), "text");
        Boomerang::Core::Graphics::Renderer::RenderText("Boomerang 3cv0.1.0-pre.4-alpha", { 0, 290, RENDER_LAYER::LAYER1 }, { 1.f, 1.f }, glm::vec3(1.f), manager.GetFont("nsjpl", 22));
        Boomerang::Core::Graphics::Renderer::RenderText(std::to_string((int)manager.fps()), { 920, 520, RENDER_LAYER::LAYER1 }, { 1.f, 1.f }, glm::vec3(0, 1, 0), manager.GetFont("nsjpl", 32));
        Boomerang::Core::Graphics::Renderer::RenderText("Chunks Rendered: " + std::to_string((int)chunks_rendered), { 0, 0, RENDER_LAYER::LAYER1 }, { 1.f, 1.f }, glm::vec3(0, 1, 0), manager.GetFont("nsjpl", 32));
        Boomerang::Core::Graphics::Renderer::RenderText("Chunks Generated: " + std::to_string(manager.GetWorld()->GetMap().size()), { 0, -30, RENDER_LAYER::LAYER1 }, { 1.f, 1.f }, glm::vec3(0, 1, 0), manager.GetFont("nsjpl", 32));
        Boomerang::Core::Graphics::Renderer::EndScene();

        Boomerang::Core::Graphics::Manager::EndRender(manager.GetWindow());
    }

    manager.shutdown();

    return 0;
}
