// Project Boomerang : engine/world/world.hpp (c) 2020-2021 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan

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

#ifndef BOOMERANG_ENGINE_WORLD_WORLD
#define BOOMERANG_ENGINE_WORLD_WORLD

// Include standard library
#include <memory>
#include <random>
#include <vector>
#include <functional>

// Include dependencies
#include <GLM/glm/glm.hpp>

// Include boomerang libraries
#include "../graphics/texture.hpp"

namespace Boomerang::Core::World {

    class World {

        /// World object

    public:

        World() = default;
        ~World();

    private:

    };

    glm::vec3 GridToPixelCoord(const glm::vec3& _GridCoord, const float _CellSize);
    glm::vec3 PixelToGridCoord(const glm::vec3& _PixlCoord, const float _CellSize);

    enum class BIOME {
        POLAR,
        TUNDRA,
        BORL_FOREST,
        COLD_DESERT,
        PLAINS,
        TEMP_FOREST,
        WARM_DESERT,
        GRASSLAND,
        SAVANNA,
        TROP_FOREST,
        RAIN_FOREST,
        OCEAN
    };

    std::ostream& operator<< (std::ostream& stream, const BIOME& biome);

    struct BIOME_TEXTURES {
        std::shared_ptr<Boomerang::Core::Graphics::Texture> test;
        std::shared_ptr<Boomerang::Core::Graphics::Texture> POLAR;
        std::shared_ptr<Boomerang::Core::Graphics::Texture> TUNDRA;
        std::shared_ptr<Boomerang::Core::Graphics::Texture> BORL_FOREST;
        std::shared_ptr<Boomerang::Core::Graphics::Texture> COLD_DESERT;
        std::shared_ptr<Boomerang::Core::Graphics::Texture> PLAINS;
        std::shared_ptr<Boomerang::Core::Graphics::Texture> TEMP_FOREST;
        std::shared_ptr<Boomerang::Core::Graphics::Texture> WARM_DESERT;
        std::shared_ptr<Boomerang::Core::Graphics::Texture> GRASSLAND;
        std::shared_ptr<Boomerang::Core::Graphics::Texture> SAVANNA;
        std::shared_ptr<Boomerang::Core::Graphics::Texture> TROP_FOREST;
        std::shared_ptr<Boomerang::Core::Graphics::Texture> RAIN_FOREST;
        std::shared_ptr<Boomerang::Core::Graphics::Texture> OCEAN;

        const std::shared_ptr<Boomerang::Core::Graphics::Texture>& GetBiomeTexture(BIOME biome) const;
    };

    const BIOME DetermineBiome(const glm::vec2& position, 
                               const std::vector<std::function<float(int, int)>>& nh,
                               const std::vector<std::function<float(int, int)>>& nt,
                               const std::vector<std::function<float(int, int)>>& nr);

    struct VoronoiPoint {

        VoronoiPoint() = default;

        VoronoiPoint(const glm::vec2& _ChunkCoord,
                     const std::function<glm::vec2(int, int)>& vf,
                     const std::vector<std::function<float(int, int)>>& nh,
                     const std::vector<std::function<float(int, int)>>& nt,
                     const std::vector<std::function<float(int, int)>>& nr);

        static std::pair<BIOME, std::shared_ptr<Boomerang::Core::Graphics::Texture>> 
                        ClosestBiome(const glm::vec2& node, const VoronoiPoint& vp, const BIOME_TEXTURES& bt);

        glm::vec2 TrueNode;
        glm::vec2 ChunkCoord;

        std::vector<glm::vec2> SurroundingVoronoi;
        std::vector<BIOME> SurroundingBiomes;

        BIOME biome;
    };
}

#endif // !BOOMERANG_ENGINE_WORLD_WORLD
