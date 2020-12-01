#include <vector>
#include <GLM/glm/glm.hpp>

#ifndef BOOMERANG_ENGINE_UNIT
#define BOOMERANG_ENGINE_UNIT
// For now, this unit represents a quad object that can hold other units as children
// To do: update the unit to holds a vector of vertices to outline the unit and replace size
class Unit {
    public:
        // Getters
        vector<unit> GetChildren();
        Unit GetParent();
        bool GetVisible();

        // Setters
        void AddChild(Unit _unit);
        void SetVisible(bool _visible);

        // Functions
        void Update();
    private:
        glm::vec4 color;
        glm::vec2 position;
        glm::vec2 size;
        std::shared_ptr<Texture>
        double rotation;
        std::shared_ptr<vector<Unit>> children;
        Unit parent;
        bool visible;
        bool shouldDisplay;
}
