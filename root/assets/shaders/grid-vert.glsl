#version 460 core

// Position of grid rect is a constant, centered at 0, 0
layout(location = 0) in vec3 a_Position;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

uniform vec2 u_Resolution;
uniform vec3 u_CameraPosition;
uniform float u_CellSize;

// Position to output to fragment shader
out vec2 v_Resolution;
out vec3 v_CamPos;
out float v_CellSize;

void main() {

    v_Resolution = u_Resolution;
    v_CamPos = u_CameraPosition;
    v_CellSize = u_CellSize;

    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}
