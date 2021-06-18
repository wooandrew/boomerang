// Project Boomerang : engine/world/chunk.cpp (c) 2020-2021 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan

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

#include "chunk.hpp"
#include <iostream>

namespace Boomerang::Core::World {

    Chunk::Chunk(const glm::vec3& _position, const float _CellSize, const float _scale, const VoronoiPoint& _vp) {

        position = _position;
        CellSize = _CellSize;
        ChunkCoord = { position.x / 8, position.y / 8 };
        vp = _vp;
        Generate(glm::vec2(_CellSize), glm::vec2(_scale));
    }

    Chunk::Chunk(const glm::vec3& _position, const glm::vec2& _CellSize, const glm::vec2& _scale, const VoronoiPoint& _vp) {
        position = _position;
        ChunkCoord = { position.x / 8, position.y / 8 };
        vp = _vp;
        Generate(_CellSize, _scale);
    }

    Chunk::~Chunk() { };

    void Chunk::Generate(const glm::vec2& _size, const glm::vec2& _scale) {

        int xPos = static_cast<int>(position.x);
        int yPos = static_cast<int>(position.y);

        for (int x = xPos - 3; x < xPos + 5; x++) {
            for (int y = yPos + 3; y > yPos - 5; y--) {

                glm::vec3 pos = { x, y, position.z };

                ASWL::eXperimental::SetHash hash(x, y);
                map.insert({ hash, std::make_shared<Node>(pos, _size, _scale) });
            }
        }
    }

    const std::map<ASWL::eXperimental::SetHash, std::shared_ptr<Node>>& Chunk::GetMap() const {
        return map;
    }

    bool Chunk::InFrame(const glm::vec3& _position, const glm::vec2& _windowSize) {

        float xMax = _position.x + (_windowSize.x / 2);
        float xMin = _position.x - (_windowSize.x / 2);
        float yMax = _position.y + (_windowSize.y / 2);
        float yMin = _position.y - (_windowSize.y / 2);

        glm::vec3 p = GridToPixelCoord(position, CellSize);

        float xMaxChunk = p.x + (CellSize / 2) + (4.f * CellSize);
        float xMinChunk = p.x - (CellSize / 2) - (3.f * CellSize);
        float yMaxChunk = p.y + (CellSize / 2) + (4.f * CellSize);
        float yMinChunk = p.y - (CellSize / 2) - (4.f * CellSize);

        if (xMinChunk >= xMax || xMaxChunk <= xMin || yMinChunk >= yMax || yMaxChunk <= yMin)
            return false;

        return true;
    }

    std::ostream& operator<< (std::ostream& stream, const Chunk& chunk) {

        for (auto const& [key, val] : chunk.map)
            stream << key << '\n';

        return stream;
    }
}
