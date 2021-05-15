// Project Boomerang : engine/world/grid.cpp (c) 2020-2021 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan

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

#include "grid.hpp"

// Include standard library
#include <iostream>
#include <future>
#include <thread>
#include <chrono>
#include <utility>

// Include boomerang libraries
#include "../math/math.hpp"
#include "world.hpp"

namespace Boomerang::Core::World {

    Grid::Grid(float _CellSize, float _scale) {

        CellSize = _CellSize;
        scale = _scale;

        seed = std::chrono::system_clock::now().time_since_epoch().count();
        mte = std::mt19937_64(seed);

        seed = mte();                       // Randomly generate seed, which is seeded by current_time
        mte = std::mt19937_64(seed);        // Create a Mersenne Twister (19937_64) generator

        TempNoises = Boomerang::Core::Math::NoiseFactory(mte, 9);
        RainNoises = Boomerang::Core::Math::NoiseFactory(mte, 6);

        LastPosition = { 0, 0, 0 };
    }

    Grid::Grid(float seed, float _CellSize, float _scale) {

        CellSize = _CellSize;
        scale = _scale;

        mte = std::mt19937_64(seed);        // Create a Mersenne Twister (19937_64) generator

        TempNoises = Boomerang::Core::Math::NoiseFactory(mte, 6);
        RainNoises = Boomerang::Core::Math::NoiseFactory(mte, 6);

        LastPosition = { 0, 0, 0 };
    }

    Grid::~Grid() { }

    void Grid::init(const glm::vec3& _position, const glm::vec2& _windowSize) {

        bt.test        = std::make_shared<Boomerang::Core::Graphics::Texture>("assets/nodes/test_125.png");
        bt.POLAR       = std::make_shared<Boomerang::Core::Graphics::Texture>("assets/nodes/polar_125.png");
        bt.TUNDRA      = std::make_shared<Boomerang::Core::Graphics::Texture>("assets/nodes/tundra_125.png");
        bt.BORL_FOREST = std::make_shared<Boomerang::Core::Graphics::Texture>("assets/nodes/borl_forest_125.png");
        bt.COLD_DESERT = std::make_shared<Boomerang::Core::Graphics::Texture>("assets/nodes/cold_desert_125.png");
        bt.PLAINS      = std::make_shared<Boomerang::Core::Graphics::Texture>("assets/nodes/plains_125.png");
        bt.TEMP_FOREST = std::make_shared<Boomerang::Core::Graphics::Texture>("assets/nodes/temp_forest_125.png");
        bt.WARM_DESERT = std::make_shared<Boomerang::Core::Graphics::Texture>("assets/nodes/warm_desert_125.png");
        bt.GRASSLAND   = std::make_shared<Boomerang::Core::Graphics::Texture>("assets/nodes/grassland_125.png");
        bt.SAVANNA     = std::make_shared<Boomerang::Core::Graphics::Texture>("assets/nodes/savanna_125.png");
        bt.TROP_FOREST = std::make_shared<Boomerang::Core::Graphics::Texture>("assets/nodes/trop_forest_125.png");
        bt.RAIN_FOREST = std::make_shared<Boomerang::Core::Graphics::Texture>("assets/nodes/rain_forest_125.png");
        bt.OCEAN       = std::make_shared<Boomerang::Core::Graphics::Texture>("assets/nodes/ocean_125.png");

        // Return pixel position as grid position
        glm::vec3 gridCoord = PixelToGridCoord(_position, CellSize);

        // Calculate xMax, xMin, yMax, yMin in terms of grid coords.
        float xMax = std::round((_position.x + _windowSize.x / 2) / CellSize);
        float xMin = std::round((_position.x - _windowSize.x / 2) / CellSize) + 1.f;        // +1 to normalize xMin
        float yMax = std::round((_position.y + _windowSize.y / 2) / CellSize) - 1.f;        // -1 to normalize yMax
        float yMin = std::round((_position.y - _windowSize.y / 2) / CellSize);

        // Calculate which chunk coord xMax, xMin, yMax, yMin are in
        float xMaxCC = std::round(xMax / 8) + 1;
        float xMinCC = std::round(xMin / 8) - 1;
        float yMaxCC = std::round(yMax / 8) + 1;
        float yMinCC = std::round(yMin / 8) - 1;

        // Calculate chunks to generate
        for (int y = yMinCC * 8; y <= yMaxCC * 8; y += 8) {
            for (int x = xMinCC * 8; x <= xMaxCC * 8; x += 8) {    
                ASWL::eXperimental::SetHash hash(x, y);
                if (map.count(hash) == 0)
                    LoadChunk(hash, _position.z);
            }
        }
    }

    void Grid::update(const glm::vec3& _position, const glm::vec2& _windowSize) {

        // Return pixel position as grid position
        glm::vec3 gridCoord = PixelToGridCoord(_position, CellSize);

        auto f = std::async(std::launch::async,
        
            [&]() {

                // Calculate xMax, xMin, yMax, yMin in terms of grid coords.
                float xMax = std::round((_position.x + _windowSize.x / 2) / CellSize);
                float xMin = std::round((_position.x - _windowSize.x / 2) / CellSize) + 1.f;        // +1 to normalize xMin
                float yMax = std::round((_position.y + _windowSize.y / 2) / CellSize) - 1.f;        // -1 to normalize yMax
                float yMin = std::round((_position.y - _windowSize.y / 2) / CellSize);

                // Calculate which chunk coord xMax, xMin, yMax, yMin are in
                float xMaxCC = std::round(xMax / 8) + 1;
                float xMinCC = std::round(xMin / 8) - 1;
                float yMaxCC = std::round(yMax / 8) + 1;
                float yMinCC = std::round(yMin / 8) - 1;

                std::map<ASWL::eXperimental::SetHash, std::shared_ptr<Chunk>> cpy = map;

                std::vector<ASWL::eXperimental::SetHash> hashList;

                // Calculate chunks to unload
                for (auto const& [key, val] : cpy) {
                    if (static_cast<int>(key.x.to_ulong()) < xMinCC * 8 || static_cast<int>(key.x.to_ulong()) > xMaxCC * 8 ||
                        static_cast<int>(key.y.to_ulong()) < yMinCC * 8 || static_cast<int>(key.y.to_ulong()) > yMaxCC * 8) {
                        hashList.push_back(key);
                    }
                }

                for (auto const& hash : hashList)
                    cpy.erase(hash);

                for (int y = yMinCC * 8; y <= yMaxCC * 8; y += 8) {
                    for (int x = xMinCC * 8; x <= xMaxCC * 8; x += 8) {
                        ASWL::eXperimental::SetHash hash(x, y);
                        if (cpy.count(hash) == 0) {

                            glm::vec3 p = { static_cast<int>(hash.x.to_ulong()), static_cast<int>(hash.y.to_ulong()), _position.z };
                            cpy.insert({ hash, std::make_shared<Chunk>(p, CellSize, 80.f / 125.f) });
                            for (auto const& [key, val] : cpy[hash]->GetMap())
                                val->SetBiome(DetermineBiome(mte, { val->GetPosition().x, val->GetPosition().y }, TempNoises, RainNoises), bt);
                        }
                    }
                }

                return cpy;
            }
        );

        if (LastPosition != gridCoord) {
            LastPosition = gridCoord;
            map = f.get();
        }
    }

    void Grid::GenerateChunk(const glm::vec3& _position) {

        ASWL::eXperimental::SetHash hash(_position.x, _position.y);
        Chunk c(_position, CellSize, 80.f / 125.f);
        map.insert({ hash, std::make_shared<Chunk>(c) });
        
        for (auto const& [key, val] : map[hash]->GetMap())
            val->SetBiome(DetermineBiome(mte, { val->GetPosition().x, val->GetPosition().y }, TempNoises, RainNoises), bt);
    }

    void Grid::LoadChunk(const ASWL::eXperimental::SetHash& hash, const float layer) {
        GenerateChunk({ static_cast<int>(hash.x.to_ulong()), static_cast<int>(hash.y.to_ulong()), layer });
    }

    void Grid::UnloadChunk(const ASWL::eXperimental::SetHash& hash) {
        map.erase(hash);
    }

    // Getters
    const float Grid::GetCellSize() const {
        return CellSize;
    }

    const std::map<ASWL::eXperimental::SetHash, std::shared_ptr<Chunk>>& Grid::GetMap() const {
        return map;
    }
}
