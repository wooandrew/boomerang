// Project Boomerang : engine/world/grid.hpp (c) 2020-2021 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan

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

#ifndef BOOMERANG_ENGINE_WORLD_GRID
#define BOOMERANG_ENGINE_WORLD_GRID

// Include standard library
#include <map>
#include <random>

// Include dependencies
#include <GLM/glm/glm.hpp>
#include "../../xperimental.hpp"

// Include boomerang libraries
#include "chunk.hpp"

namespace Boomerang::Core::World {

    class Grid {

        /// World grid

    public:

        Grid(float _CellSize = 80.f, float _scale = 1.f);
        Grid(float seed, float _CellSize = 80.f, float _scale = 1.f);
        ~Grid();

        void init(const glm::vec3& _position, const glm::vec2& _windowSize);
        void update(const glm::vec3& _position, const glm::vec2& _windowSize);

        // Getters
        const float GetCellSize() const;
        const std::map<ASWL::eXperimental::SetHash, std::shared_ptr<Chunk>>& GetMap() const;

    private:

        unsigned long long seed;
        
        float CellSize;
        float scale;

        void GenerateChunk(const glm::vec3& _position);
        void LoadChunk(const ASWL::eXperimental::SetHash& hash, const float layer);
        void UnloadChunk(const ASWL::eXperimental::SetHash& hash);

        std::map<ASWL::eXperimental::SetHash, std::shared_ptr<Chunk>> map;     // Loaded chunk map

        std::mt19937_64 mte;
        std::vector<std::function<float(int, int)>> HeightNoises;
        std::vector<std::function<float(int, int)>> TempNoises;
        std::vector<std::function<float(int, int)>> RainNoises;

        std::function<glm::vec2(int, int)> VoronoiFunction;

        glm::vec3 LastPosition;

        BIOME_TEXTURES bt;
    };
}

#endif // !BOOMERANG_ENGINE_WORLD_GRID
