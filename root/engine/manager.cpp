// Project Boomerang : engine/manager.cpp (c) 2020-2021 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan

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

#include "manager.hpp"

// Include standard library
#include <atomic>
#include <chrono>
#include <thread>

// Include dependencies
#include <GLM/glm/gtc/matrix_transform.hpp>
#include <ASWL/logger.hpp>

// Include boomerang libraries
#include "input/mouse.hpp"
#include "input/keyboard.hpp"
#include "graphics/manager.hpp"
#include "graphics/renderer.hpp"

namespace Boomerang::Core {

    Manager::Manager() {
        DefaultCameraOrtho = glm::ortho(-500, 500, -309, 309);
        state = GAME_STATE::RUN;

        world_initialized = false;
    }

    Manager::~Manager() {
        state = GAME_STATE::STOP;
        //shutdown();
    }

    int Manager::init() {

        // Initialize the game engine
        if (engine.init() != 0) {
            ASWL::Logger::logger("  E  ", "Fatal Error: Failed to initialize game engine.");
            return -1;
        }
        else
            ASWL::Logger::logger("  E  ", "Engine initialization success. All systems go!");

        // Initialize mouse with origin
        Input::Mouse::init({ engine.GetWindowDimensions().x / 2, engine.GetWindowDimensions().y / 2 });

        // Initialize graphics manager
        Graphics::Manager::init();

        Graphics::Manager::SetClearColor(glm::vec4(0.6));

        // Initialize 2d renderer
        Graphics::Renderer::init(engine.GetWindowDimensions());

        // Set default camera ortho to fit window dimensions
        DefaultCameraOrtho = glm::ortho(-engine.GetWindowDimensions().x / 2.f, engine.GetWindowDimensions().x / 2.f,
                                        -engine.GetWindowDimensions().y / 2.f, engine.GetWindowDimensions().y / 2.f);

        auto NormalizedCameraOrtho = glm::ortho(-1, 1, -1, 1);

        // Create default cameras (main, text, debug/grid)
        cameras.insert({ "main_0", std::make_unique<Graphics::OrthoCam>(NormalizedCameraOrtho, 5.f) });
        cameras.insert({ "grid_0", std::make_unique<Graphics::OrthoCam>(DefaultCameraOrtho, 500.f) });
        cameras.insert({ "text_0", std::make_unique<Graphics::OrthoCam>(DefaultCameraOrtho, 500.f) });

        // Lock all cameras initially
        for (auto const& [key, val] : cameras)
            val->SetLock(false);

        // Create default fonts
        FontLibrary.insert({ "nsjpl", std::make_unique<Graphics::FontLibrary>("nsjpl", "assets/fonts/nsjpl.otf") });
        FontLibrary["nsjpl"]->AddSize(32);
        FontLibrary["nsjpl"]->AddSize(56);

        ASWL::Utilities::FramesPerSecond::UpdateFPS();

        return 0;
    }

    void Manager::InitializeWorld() {

        // Initialize world
        world = std::make_unique<World::Grid>(40);

        // Terrain generation flag
        std::atomic<bool> FinishedWorldInit = false;

        // Generate terrain
        auto f = std::thread(
            [&]() {
                world->init(cameras["main_0"]->GetPosition(), engine.GetWindowDimensions());
                FinishedWorldInit = true;
            }
        );

        f.detach();

        cameras["main_0"]->SetLock(true);
        auto start = std::chrono::high_resolution_clock::now();

        // Wait while world initializes
        while (!FinishedWorldInit && run()) {

            if (Boomerang::Core::Input::Keyboard::KeyIsPressed(GLFW_KEY_ESCAPE))        // QUIT
                state = Boomerang::Core::Manager::GAME_STATE::STOP;

            // Update clocks
            DeltaTime.UpdateDeltaTime();
            ASWL::Utilities::FramesPerSecond::UpdateFPS();

            // Update engine (poll events)
            engine.update();

            // Update cameras
            for (auto const& [key, val] : cameras) {
                if (!val->locked())
                    val->update(dt());
            }

            Graphics::Manager::BeginRender();

            Graphics::Renderer::StartScene(cameras["main_0"], "text");
            Boomerang::Core::Graphics::Renderer::RenderText(BUILD_VERSION, { 0, 290, 0.1f }, { 1.f, 1.f }, glm::vec3(1.f), GetFont("nsjpl", 22));
            Boomerang::Core::Graphics::Renderer::RenderText(std::to_string((int)fps()), { 920, 520, 0.1f }, { 1.f, 1.f }, glm::vec3(0, 1, 0), GetFont("nsjpl", 32));
            Boomerang::Core::Graphics::Renderer::RenderText("Generating the world...", { 0, 50, 0.1f }, { 1.f, 1.f }, glm::vec3(1), GetFont("nsjpl", 32));
            Graphics::Renderer::EndScene();

            Graphics::Renderer::StartScene(cameras["grid_0"], "dots");
            Graphics::Renderer::LoadingDots(engine.GetWindowDimensions(), 5, 12.f, 3.f, glm::vec4(0.9), std::chrono::high_resolution_clock::now() - start);
            Graphics::Renderer::EndScene();

            Graphics::Manager::EndRender(engine.GetWindow());
        }

        cameras["main_0"]->SetLock(false);
        world_initialized = true;

        ASWL::Utilities::FramesPerSecond::UpdateFPS();
    }

    void Manager::shutdown() {
        Graphics::Renderer::shutdown();
        Graphics::Manager::shutdown();
    }

    const bool Manager::run() {
        return !glfwWindowShouldClose(engine.GetWindow()) && state == GAME_STATE::RUN;
    }

    void Manager::update() {

        if (Boomerang::Core::Input::Keyboard::KeyIsPressed(GLFW_KEY_ESCAPE))        // QUIT
            state = Boomerang::Core::Manager::GAME_STATE::STOP;

        // Update clocks
        DeltaTime.UpdateDeltaTime();
        ASWL::Utilities::FramesPerSecond::UpdateFPS();
        
        // Update engine (poll events)
        engine.update();

        // Update cameras
        for (auto const& [key, val] : cameras) {
            if (!val->locked())
                val->update(dt());
        }

        // Update world
        if (world_initialized)
            world->update(cameras["main_0"]->GetPosition(), engine.GetWindowDimensions());
    }

    const float Manager::dt() {
        return static_cast<float>(DeltaTime.GetDeltaTime());
    }
    const float Manager::fps() {
        return static_cast<float>(ASWL::Utilities::FramesPerSecond::GetFPS());
    }

    const bool Manager::GetWorldInitialized() const {
        return world_initialized;
    }

    const std::unique_ptr<Graphics::OrthoCam>& Manager::GetCamera(const std::string& _name) {
        return cameras[_name];
    }
    const Graphics::Font& Manager::GetFont(const std::string& _name, int _size) {
        return FontLibrary[_name]->GetFont(_size);
    }
    const std::unique_ptr<World::Grid>& Manager::GetWorld() const {
        return world;
    }

    const glm::vec2& Manager::GetWindowDimensions() const {
        return engine.GetWindowDimensions();
    }
    GLFWwindow* Manager::GetWindow() {
        return engine.GetWindow();
    }
}
