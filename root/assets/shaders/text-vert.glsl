#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in float a_TexSlot;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexSlot;

void main() {
    
    v_TexCoord = a_TexCoord;
    v_Color = a_Color;
    v_TexSlot = a_TexSlot;

    vec3 Position = vec3(a_Position.x,  -a_Position.y, a_Position.z);

    gl_Position = u_ViewProjection * u_Transform * vec4(Position, 1.0);
}
