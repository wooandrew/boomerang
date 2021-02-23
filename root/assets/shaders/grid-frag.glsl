#version 460 core

layout(location = 0) out vec4 color;

in vec3 v_CamPos;
in float v_CellSize;

void main() {

    vec2 pos = floor((gl_FragCoord.xy + v_CamPos.xy - vec2(v_CellSize / 2.f)) / v_CellSize);    
    // PatternMask = mod(pos.x + pos.y, 2.0);
    color = mod(pos.x + pos.y, 2.0) * vec4(1.0, 1.0, 1.0, 1.0);
}
