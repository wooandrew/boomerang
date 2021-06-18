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
using namespace Boomerang::Core::Graphics::Renderer::RENDER_LAYER;

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

    std::shared_ptr<Boomerang::Core::Graphics::Texture> t0 = std::make_shared<Boomerang::Core::Graphics::Texture>("assets/nodes/ocean_125.png");
    //manager.InitializeWorld();

    while (manager.run()) {

        manager.update();

        Boomerang::Core::Graphics::Manager::BeginRender();

        Boomerang::Core::Graphics::Renderer::StartScene(manager.GetCamera("main_0"));
        Boomerang::Core::Graphics::Renderer::DrawQuad({{ 0.f, 0.f, LAYER1 }, { 20.f, 20.f }, { 1.f, 0.f, 0.f, 1.f }, 0 });
        Boomerang::Core::Graphics::Renderer::EndScene();

        Boomerang::Core::Graphics::Renderer::StartScene(manager.GetCamera("text_0"), "text_old");
        Boomerang::Core::Graphics::Renderer::RenderTextOld("YOLO!", { 0.f, 0.f, LAYER1 }, { 1.f, 1.f }, { 1.f, 0.f, 0.f, 1.f }, manager.GetFont("nsjpl", 48));
        Boomerang::Core::Graphics::Renderer::EndScene();

        //int chunks_rendered = 0;
        //
        //if (manager.GetWorldInitialized()) {
        //
        //    Boomerang::Core::Graphics::Renderer::StartScene(manager.GetCamera("main_0"));
        //
        //    for (auto const& [key, chunk] : manager.GetWorld()->GetMap()) {
        //
        //        if (chunk->InFrame(manager.GetCamera("main_0")->GetPosition(), manager.GetWindowDimensions())) {
        //            Boomerang::Core::Graphics::Renderer::RenderChunk(chunk, manager.GetWindowDimensions(), manager.GetCamera("main_0")->GetPosition(), manager.GetWorld()->GetCellSize());
        //            chunks_rendered++;
        //        }
        //    }
        //
        //    Boomerang::Core::Graphics::Renderer::EndScene();
        //}
        //
        //Boomerang::Core::Graphics::Renderer::StartScene(manager.GetCamera("grid_0"), "grid");
        //Boomerang::Core::Graphics::Renderer::RenderGrid(manager.GetWindowDimensions(), manager.GetCamera("main_0")->GetPosition(), /*manager.GetWorld()->GetCellSize()*/ 40);
        //Boomerang::Core::Graphics::Renderer::EndScene();
        //
        //Boomerang::Core::Graphics::Renderer::StartScene(manager.GetCamera("text_0"), "text_old");
        ////Boomerang::Core::Graphics::Renderer::RenderTextOld(Boomerang::Core::BUILD_VERSION, { 0, 290, LAYER1 }, { 1.f, 1.f }, glm::vec4(1.f), manager.GetFont("nsjpl", 22));
        ////Boomerang::Core::Graphics::Renderer::RenderText(std::to_string((int)manager.fps()), { 920, 520, LAYER1 }, { 1.f, 1.f }, glm::vec4(0, 1, 0, 1), manager.GetFont("nsjpl", 32));
        ////Boomerang::Core::Graphics::Renderer::RenderText("Chunks Rendered: " + std::to_string((int)chunks_rendered), { 0, 0, LAYER1 }, { 1.f, 1.f }, glm::vec4(0, 1, 0, 1), manager.GetFont("nsjpl", 32));
        ////Boomerang::Core::Graphics::Renderer::RenderText("Chunks Generated: " + std::to_string(manager.GetWorld()->GetMap().size()), { 0, -30, LAYER1 }, { 1.f, 1.f }, glm::vec4(0, 1, 0, 1), manager.GetFont("nsjpl", 32));
        //Boomerang::Core::Graphics::Renderer::EndScene();
        
        Boomerang::Core::Graphics::Manager::EndRender(manager.GetWindow());
    }

    manager.shutdown();

    return 0;
}
