// Project Boomerang : engine/engine.cpp (c) 2020 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan

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
#include "unit.hpp"
#include <GLM/glm/glm.hpp>
#include "../graphics/texture.hpp"
#include <vector>
#include <memory>

namespace Boomerang::Core::Units { 

	// Constructor
    unit(glm::vec2 _position, glm::vec2 _size, glm::vec4 color, std::shared_ptr<Texture> _texture) {
        position = _position;
        size = _size;
        color = _color;
        texture = _texture;
        rotation = 0.0;

        displayer = nullptr;
        manipulator = nullptr;

        // initialize the children and vertices vectors to empty vectors
        parent = nullptr;

        // for now
        vertices = nullptr;
    }

    // Getters
    std::shared_ptr<vector<unit>> unit::GetChildren() {
        return children;
    }

    std::shared_ptr<unit> unit::GetParent() {
        return parent;
    }

    bool unit::GetVisible() {
        return visible;
    }

    bool unit::GetShouldDisplay() {
        return shouldDisplay;
    }

    glm::vec2 unit::GetPosition() {
        return position;
    }

    glm::vec2 unit::GetSize() {
        return size;
    }

    glm::vec4 unit::GetColor() {
        return color;
    }

    // Setters
    void unit::AddChild(std::shared_ptr<unit> _unit) {
        children.push_back(_unit);
        _unit.SetParent(this);
    }

    void unit::RemoveChild(std::shared_ptr<unit> _unit) {
		for (int i = 0; i < children.size(); i++) {
            if (children[i] == unit) {
                children[i] = children[children.size() - 1];
                children.pop_back();
                break;
            }
        }
    }

    void unit::SetParent(std::shared_ptr<unit> _parent) {
        parent = _parent;
    }

    void unit::SetVisible(bool _visible) {
        visible = _visible;
    }

    void unit::SetShouldDisplay(bool _shouldDisplay) {
        shouldDisplay = _shouldDisplay;
    }

    // Functions

    void unit::Update() {
        manipulator.Update(this);
    }

    void unit::Display() {
        if (visible && shouldDisplay) {
            displayer.Display();
            for (unit _unit : children) {
                _unit.Display();
            }
        }
    }
    
}

