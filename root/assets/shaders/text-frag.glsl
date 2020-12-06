/*#version 460 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main() {

    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(u_Texture, v_TexCoord).r);
    color = u_Color * sampled;

    // Alpha channel handling
    // if(color.a < 0.1) discard;
}
*/
#version 460 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main() {

    color = texture(u_Texture, v_TexCoord * 1.0) * u_Color;
    
    // Alpha channel handling
    if(color.a < 0.1) discard;
}
