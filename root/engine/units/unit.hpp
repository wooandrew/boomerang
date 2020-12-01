#include <vector>
#include <GLM/glm/glm.hpp>
#include <memory>

#ifndef BOOMERANG_ENGINE_UNIT
#define BOOMERANG_ENGINE_UNIT
// For now, this unit represents a quad object that can hold other units as children
// To do: update the unit to holds a vector of vertices to outline the unit and replace size
namespace Boomerang::Core::Units {

    class unit {

    public:

        // Constructor
        unit(glm::vec2 _position, glm::vec2 _size, glm::vec4 color, std::shared_ptr<Texture> _texture);

        // Getters
        std::shared_ptr<vector<unit>> GetChildren();
        std::shared_ptr<unit> GetParent();
        bool GetVisible();
        glm::vec2 GetPosition();
        glm::vec2 GetSize();
        glm::vec4 GetColor();

        // Setters
		void AddChild(std::shared_ptr<unit> _unit);
        void RemoveChild(std::shared_ptr<unit> _unit)
        void SetVisible(bool _visible);
        void SetParent(std::shared_ptr<unit> _parent);
        void SetShouldDisplay(bool _shouldDisplay);

		// Functions
		void Update();
        void Display();

    private:
        glm::vec4 color; // (r, g, b, a)
        glm::vec2 position; // (x, y)
        glm::vec2 size; // (width, height)
        double rotation; // in radians

        std::shared_ptr<vector<vertex>> vertices;
        std::shared_ptr<Texture> texture;
        std::shared_ptr<unitDisplay> displayer;
        std::shared_ptr<unitManipulator> manipulator;

        std::shared_ptr<vector<unit>> children;
        std::share_ptr<unit> parent;

        bool visible;
        bool shouldDisplay;
    };
}