#version 460 core

layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexSlot;

uniform vec4 u_Color;
uniform sampler2D u_Textures[32];

void main() {

    vec4 o_Color = u_Color * v_Color;

    if (v_Color.x == 0 || v_Color.y == 0 || v_Color.z == 0)
        o_Color = u_Color;

    switch (int(v_TexSlot)) {

        case  0: color = vec4(1.0, 1.0, 1.0, texture(u_Textures[0],  v_TexCoord).r) * o_Color;
        case  1: color = vec4(1.0, 1.0, 1.0, texture(u_Textures[1],  v_TexCoord).r) * o_Color;
        case  2: color = vec4(1.0, 1.0, 1.0, texture(u_Textures[2],  v_TexCoord).r) * o_Color;
        case  3: color = vec4(1.0, 1.0, 1.0, texture(u_Textures[3],  v_TexCoord).r) * o_Color;
        case  4: color = vec4(1.0, 1.0, 1.0, texture(u_Textures[4],  v_TexCoord).r) * o_Color;
        case  5: color = vec4(1.0, 1.0, 1.0, texture(u_Textures[5],  v_TexCoord).r) * o_Color;
        case  6: color = vec4(1.0, 1.0, 1.0, texture(u_Textures[6],  v_TexCoord).r) * o_Color;
        case  7: color = vec4(1.0, 1.0, 1.0, texture(u_Textures[7],  v_TexCoord).r) * o_Color;
        case  8: color = vec4(1.0, 1.0, 1.0, texture(u_Textures[8],  v_TexCoord).r) * o_Color;
        case  9: color = vec4(1.0, 1.0, 1.0, texture(u_Textures[9],  v_TexCoord).r) * o_Color;
        case 10: color = vec4(1.0, 1.0, 1.0, texture(u_Textures[10], v_TexCoord).r) * o_Color;
        case 11: color = vec4(1.0, 1.0, 1.0, texture(u_Textures[11], v_TexCoord).r) * o_Color;
        case 12: color = vec4(1.0, 1.0, 1.0, texture(u_Textures[12], v_TexCoord).r) * o_Color;
        case 13: color = vec4(1.0, 1.0, 1.0, texture(u_Textures[13], v_TexCoord).r) * o_Color;
        case 14: color = vec4(1.0, 1.0, 1.0, texture(u_Textures[14], v_TexCoord).r) * o_Color;
        case 15: color = vec4(1.0, 1.0, 1.0, texture(u_Textures[15], v_TexCoord).r) * o_Color;
        case 16: color = vec4(1.0, 1.0, 1.0, texture(u_Textures[16], v_TexCoord).r) * o_Color;
        case 17: color = vec4(1.0, 1.0, 1.0, texture(u_Textures[17], v_TexCoord).r) * o_Color;
        case 18: color = vec4(1.0, 1.0, 1.0, texture(u_Textures[18], v_TexCoord).r) * o_Color;
        case 19: color = vec4(1.0, 1.0, 1.0, texture(u_Textures[19], v_TexCoord).r) * o_Color;
        case 20: color = vec4(1.0, 1.0, 1.0, texture(u_Textures[20], v_TexCoord).r) * o_Color;
        case 21: color = vec4(1.0, 1.0, 1.0, texture(u_Textures[21], v_TexCoord).r) * o_Color;
        case 22: color = vec4(1.0, 1.0, 1.0, texture(u_Textures[22], v_TexCoord).r) * o_Color;
        case 23: color = vec4(1.0, 1.0, 1.0, texture(u_Textures[23], v_TexCoord).r) * o_Color;
        case 24: color = vec4(1.0, 1.0, 1.0, texture(u_Textures[24], v_TexCoord).r) * o_Color;
        case 25: color = vec4(1.0, 1.0, 1.0, texture(u_Textures[25], v_TexCoord).r) * o_Color;
        case 26: color = vec4(1.0, 1.0, 1.0, texture(u_Textures[26], v_TexCoord).r) * o_Color;
        case 27: color = vec4(1.0, 1.0, 1.0, texture(u_Textures[27], v_TexCoord).r) * o_Color;
        case 28: color = vec4(1.0, 1.0, 1.0, texture(u_Textures[28], v_TexCoord).r) * o_Color;
        case 29: color = vec4(1.0, 1.0, 1.0, texture(u_Textures[29], v_TexCoord).r) * o_Color;
        case 30: color = vec4(1.0, 1.0, 1.0, texture(u_Textures[30], v_TexCoord).r) * o_Color;
        case 31: color = vec4(1.0, 1.0, 1.0, texture(u_Textures[31], v_TexCoord).r) * o_Color;
    }
    
    color = o_Color;

    // Alpha channel handling
    if(color.a < 0.1) discard;
}
