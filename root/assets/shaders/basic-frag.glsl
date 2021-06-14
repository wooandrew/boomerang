#version 460 core

layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexSlot;

uniform sampler2D u_Texture;
uniform vec4 u_Color;

void main() {

    vec4 o_Color = u_Color * v_Color;

    if (v_Color.x == 0 || v_Color.y == 0 || v_Color.z == 0)
        o_Color = u_Color;

    color = texture(u_Texture, v_TexCoord) * v_Color;

    // Alpha channel handling
    if(color.a < 0.1) discard;
}
