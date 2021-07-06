#version 460 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main() {

    color = vec4(texture(u_Texture, v_TexCoord).r, 1.0, 1.0, 1.0) * u_Color;
    
    // Alpha channel handling
    if(color.a < 0.1) discard;
}
