// Project Boomerang : engine/world/world.cpp (c) 2020-2021 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan

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

#include "world.hpp"

#include <iostream>
#include "../math/math.hpp"

namespace Boomerang::Core::World {

    glm::vec3 GridToPixelCoord(const glm::vec3& _GridCoord, const float _CellSize) {

        glm::vec3 ret = {};

        ret.x = (_GridCoord.x * _CellSize) - (_CellSize / 2);
        ret.y = (_GridCoord.y * _CellSize) + (_CellSize / 2);
        ret.z = _GridCoord.z;

        return ret;
    }

    glm::vec3 PixelToGridCoord(const glm::vec3& _PixlCoord, const float _CellSize) {

        glm::vec3 ret = {};

        ret.x = std::round(_PixlCoord.x / _CellSize);
        ret.y = std::round(_PixlCoord.y / _CellSize);
        ret.z = _PixlCoord.z;

        return ret;
    }

    std::ostream& operator<< (std::ostream& stream, const BIOME& biome) {

        if (biome == BIOME::POLAR)
            stream << "POLAR";
        if (biome == BIOME::TUNDRA)
            stream << "TUNDRA";
        if (biome == BIOME::BORL_FOREST)
            stream << "BORL_FOREST";
        if (biome == BIOME::COLD_DESERT)
            stream << "COLD_DESERT";
        if (biome == BIOME::PLAINS)
            stream << "PLAINS";
        if (biome == BIOME::TEMP_FOREST)
            stream << "TEMP_FOREST";
        if (biome == BIOME::WARM_DESERT)
            stream << "WARM_DESERT";
        if (biome == BIOME::GRASSLAND)
            stream << "GRASSLAND";
        if (biome == BIOME::SAVANNA)
            stream << "SAVANNA";
        if (biome == BIOME::TROP_FOREST)
            stream << "TROP_FOREST";
        if (biome == BIOME::RAIN_FOREST)
            stream << "RAIN_FOREST";
        if (biome == BIOME::OCEAN)
            stream << "OCEAN";

        return stream;
    }

    const std::shared_ptr<Boomerang::Core::Graphics::Texture>& BIOME_TEXTURES::GetBiomeTexture(BIOME biome) const {

        if (biome == BIOME::POLAR)
            return POLAR;
        if (biome == BIOME::TUNDRA)
            return TUNDRA;
        if (biome == BIOME::BORL_FOREST)
            return BORL_FOREST;
        if (biome == BIOME::COLD_DESERT)
            return COLD_DESERT;
        if (biome == BIOME::PLAINS)
            return PLAINS;
        if (biome == BIOME::TEMP_FOREST)
            return TEMP_FOREST;
        if (biome == BIOME::WARM_DESERT)
            return WARM_DESERT;
        if (biome == BIOME::GRASSLAND)
            return GRASSLAND;
        if (biome == BIOME::SAVANNA)
            return SAVANNA;
        if (biome == BIOME::TROP_FOREST)
            return TROP_FOREST;
        if (biome == BIOME::RAIN_FOREST)
            return RAIN_FOREST;
        if (biome == BIOME::OCEAN)
            return OCEAN;
    }

    const BIOME DetermineBiome(const glm::vec2& position,
                               const std::vector<std::function<float(int, int)>>& nh,
                               const std::vector<std::function<float(int, int)>>& nt,
                               const std::vector<std::function<float(int, int)>>& nr) {

        // Perlin for Height
        float height = Boomerang::Core::Math::Perlin2D(position.x, position.y, 0.25, 0.5f, nh);

        // Perlin for Temerature
        float temp = Boomerang::Core::Math::Perlin2D(position.x, position.y, 0.25, 0.25, nt);

        // Perlin for Rainfall
        float rain = Boomerang::Core::Math::Perlin2D(position.x, position.y, 0.25, 0.0f, nr);

        if (height > 0.0) {

            if (temp < -0.35)
                return BIOME::POLAR;
            else if (temp < 0.00)
                return BIOME::TUNDRA;
            else if (temp < 0.35)
                return BIOME::BORL_FOREST;

            else if (temp < 0.85) {

                if (rain < -0.33)
                    return BIOME::COLD_DESERT;
                else if (rain < 0.33)
                    return BIOME::PLAINS;
                else if (rain < 1.00)
                    return BIOME::TEMP_FOREST;
            }

            else if (temp < 1.25) {

                if (rain < -0.60)
                    return BIOME::WARM_DESERT;
                else if (rain < -0.20)
                    return BIOME::GRASSLAND;
                else if (rain < 0.20)
                    return BIOME::SAVANNA;
                else if (rain < 0.60)
                    return BIOME::TROP_FOREST;
                else if (rain < 1.00)
                    return BIOME::RAIN_FOREST;
            }
        }
        
        return BIOME::OCEAN;
    }

    VoronoiPoint::VoronoiPoint(const glm::vec2& _ChunkCoord, 
                               const std::function<glm::vec2(int, int)>& vf,
                               const std::vector<std::function<float(int, int)>>& nh,
                               const std::vector<std::function<float(int, int)>>& nt,
                               const std::vector<std::function<float(int, int)>>& nr) {

        ChunkCoord = _ChunkCoord;

        glm::vec2 temp = vf(ChunkCoord.x, ChunkCoord.y);
        TrueNode = glm::vec2((_ChunkCoord.x * 8) + temp.x, (_ChunkCoord.y * 8) + temp.y);
        biome = DetermineBiome(ChunkCoord, nh, nt, nr);

        //std::cout << "CC: " << ChunkCoord.x << " " << ChunkCoord.y << std::endl;
        //std::cout << "TN: " << TrueNode.x << " " << TrueNode.y << std::endl;

        // Top
        temp = vf(ChunkCoord.x - 1, ChunkCoord.y + 1);
        SurroundingVoronoi.push_back({ ((ChunkCoord.x - 1) * 8) + temp.x, ((ChunkCoord.y + 1) * 8) + temp.y });
        SurroundingBiomes.push_back(DetermineBiome({ ChunkCoord.x - 1, ChunkCoord.y + 1 }, nh, nt, nr));

        temp = vf(ChunkCoord.x, ChunkCoord.y + 1);
        SurroundingVoronoi.push_back({ (ChunkCoord.x * 8) + temp.x, ((ChunkCoord.y + 1) * 8) + temp.y });
        SurroundingBiomes.push_back(DetermineBiome({ ChunkCoord.x    , ChunkCoord.y + 1 }, nh, nt, nr));

        temp = vf(ChunkCoord.x + 1, ChunkCoord.y + 1);
        SurroundingVoronoi.push_back({ ((ChunkCoord.x + 1) * 8) + temp.x, ((ChunkCoord.y + 1) * 8) + temp.y });
        SurroundingBiomes.push_back(DetermineBiome({ ChunkCoord.x + 1, ChunkCoord.y + 1 }, nh, nt, nr));

        // Mid
        temp = vf(ChunkCoord.x - 1, ChunkCoord.y);
        SurroundingVoronoi.push_back({ ((ChunkCoord.x - 1) * 8) + temp.x, (ChunkCoord.y * 8) + temp.y });
        SurroundingBiomes.push_back(DetermineBiome({ ChunkCoord.x - 1, ChunkCoord.y }, nh, nt, nr));
        
        temp = vf(ChunkCoord.x + 1, ChunkCoord.y);
        SurroundingVoronoi.push_back({ ((ChunkCoord.x + 1) * 8) + temp.x, (ChunkCoord.y * 8) + temp.y });
        SurroundingBiomes.push_back(DetermineBiome({ ChunkCoord.x + 1, ChunkCoord.y }, nh, nt, nr));

        // Bot
        temp = vf(ChunkCoord.x - 1, ChunkCoord.y - 1);
        SurroundingVoronoi.push_back({ ((ChunkCoord.x - 1) * 8) + temp.x, ((ChunkCoord.y - 1) * 8) + temp.y });
        SurroundingBiomes.push_back(DetermineBiome({ ChunkCoord.x - 1, ChunkCoord.y - 1 }, nh, nt, nr));

        temp = vf(ChunkCoord.x, ChunkCoord.y - 1);
        SurroundingVoronoi.push_back({ (ChunkCoord.x * 8) + temp.x, ((ChunkCoord.y - 1) * 8) + temp.y });
        SurroundingBiomes.push_back(DetermineBiome({ ChunkCoord.x    , ChunkCoord.y - 1 }, nh, nt, nr));

        temp = vf(ChunkCoord.x + 1, ChunkCoord.y - 1);
        SurroundingVoronoi.push_back({ ((ChunkCoord.x + 1) * 8) + temp.x, ((ChunkCoord.y - 1) * 8) + temp.y });
        SurroundingBiomes.push_back(DetermineBiome({ ChunkCoord.x + 1, ChunkCoord.y - 1 }, nh, nt, nr));
    }
    
    std::pair<BIOME, std::shared_ptr<Boomerang::Core::Graphics::Texture>> 
        VoronoiPoint::ClosestBiome(const glm::vec2& node, const VoronoiPoint& vp, const BIOME_TEXTURES& bt) {

        // index with the minimum distance
        int mindex = 0;
        float mindist = Boomerang::Core::Math::Distance(node, vp.SurroundingVoronoi[0]);

        for (int i = 1; i < vp.SurroundingVoronoi.size(); i++) {

            float dist = Boomerang::Core::Math::Distance(node, vp.SurroundingVoronoi[i]);

            if (dist < mindist) {
                mindist = dist;
                mindex = i;
            }
        }
            
        if (Boomerang::Core::Math::Distance(node, vp.TrueNode) < mindist)
            return std::make_pair(vp.biome, bt.GetBiomeTexture(vp.biome));
        else
            return std::make_pair(vp.SurroundingBiomes[mindex], bt.GetBiomeTexture(vp.SurroundingBiomes[mindex]));
    }
}
