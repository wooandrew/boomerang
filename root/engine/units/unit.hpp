// Project Boomerang : unit/unit.hpp (c) 2020 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan

/* Modified MIT License
 *
 * Copyright 2020 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan
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

#include <vector>
#include <memory>

#include <GLM/glm/glm.hpp>

#include "../graphics/texture.hpp"
#include "../graphics/vertex.hpp"

#ifndef BOOMERANG_ENGINE_UNIT_UNIT
#define BOOMERANG_ENGINE_UNIT_UNIT
// For now, this unit represents a quad object that can hold other units as children
// To do: update the unit to holds a vector of vertices to outline the unit and replace size
namespace Boomerang::Core::Units {

    class Unit {

    public:

        // Constructor
        Unit(glm::vec3 _position, glm::vec2 _size, glm::vec4 color, std::shared_ptr<Texture> _texture);

        // Getters
        std::vector<std::shared_ptr<Unit>> GetChildren();
        std::shared_ptr<Unit> GetParent();
        bool GetVisible();
        glm::vec3 GetPosition();
        glm::vec2 GetSize();
        glm::vec4 GetColor();
        std::vector<std::shared_ptr<std::string>> GetTags();

        // Setters
		void AddChild(std::shared_ptr<Unit> _unit);
        void RemoveChild(std::shared_ptr<Unit> _unit)
        void SetVisible(bool _visible);
        void SetParent(std::shared_ptr<Unit> _parent);
        void SetShouldDisplay(bool _shouldDisplay);
		
		// Functions
		virtual void Update();
		virtual void Display();
		
    private:
        glm::vec4 color; // (r, g, b, a)
        glm::vec3 position; // (x, y, z)
        glm::vec2 size; // (width, height)
        double rotation; // in radians

        std::vector<std::shared_ptr<vertex>> vertices;
        std::shared_ptr<Texture> texture;

        std::vector<std::shared_ptr<Unit>> children;
        std::shared_ptr<Unit> parent;

        bool visible;
        bool shouldDisplay;
        bool displayVertices;

        std::string ID; // this_cool_item
        std::string displayName; // This Cool Item
        vector<shared_ptr<std::string>> tags;
    };
}