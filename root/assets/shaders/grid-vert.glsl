#version 460 core

// Position of grid rect is a constant, centered at 0, 0
layout(location = 0) in vec3 a_Position;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
uniform float u_CellSize;
uniform vec3 u_CameraPosition;

// Position to output to fragment shader
out float v_CellSize;
out vec3 v_CamPos;

void main() { 
    v_CamPos = u_CameraPosition;
    v_CellSize = u_CellSize;
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}
