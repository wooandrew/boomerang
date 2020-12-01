#include <vector>
#include <GLM/glm/glm.hpp>
#include "../graphics/texture.hpp"

#ifndef BOOMERANG_ENGINE_UNITDISPLAY
#define BOOMERANG_ENGINE_UNITDISPLAY
// For now, this unit represents a quad object that can hold other units as children
// To do: update the unit to holds a vector of vertices to outline the unit and replace size
class UnitDisplay {
    public:
        void Display(Unit& unit)) const;
    private:
        bool displayVertices;
}