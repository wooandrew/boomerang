// Project Boomerang : engine/manager.hpp (c) 2020-2021 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan

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

#pragma once

#ifndef BOOMERANG_ENGINE_MANAGER
#define BOOMERANG_ENGINE_MANAGER

// Include standard library
#include <map>
#include <string>
#include <memory>

// Include dependencies
#include <GLM/glm/glm.hpp>
#include <ASWL/utilities.hpp>

// Include boomerang libraries
#include "engine.hpp"
#include "graphics/font.hpp"
#include "graphics/camera/orthocam.hpp"
#include "world/grid.hpp"

namespace Boomerang::Core {

    constexpr const char* BUILD_VERSION = "Boomerang 11rv0.1.0-pre.4-alpha";

    class Manager {

        /// This class manages background tasks for the game. Process Manager.

    public:

        Manager();
        ~Manager();

        int init();
        void shutdown();

        void InitializeWorld();      // prototype // this should eventually take in savefile or seed/cellsize

        enum class GAME_STATE {
            RUN,
            STOP
        }; GAME_STATE state;

        const bool run();
        void update();

        // Getters
        const float dt();
        const float fps();

        const bool GetWorldInitialized() const;

        const std::unique_ptr<Graphics::OrthoCam>& GetCamera(const std::string& _name);
        const std::shared_ptr<Graphics::Font>& GetFont(const std::string& _name, int _size);
        const std::unique_ptr<World::Grid>& GetWorld() const;
        
        const glm::vec2& GetWindowDimensions() const;
        GLFWwindow* GetWindow();

    private:

        Engine engine;
        
        glm::mat4 DefaultCameraOrtho;
        std::map<std::string, std::unique_ptr<Graphics::OrthoCam>> cameras;
        std::map<std::string, std::unique_ptr<Graphics::FontLibrary>> FontLibrary;

        bool world_initialized;
        std::unique_ptr<World::Grid> world;

        ASWL::Utilities::DeltaTime DeltaTime;
    };
}

#endif // !BOOMERANG_ENGINE_MANAGER
