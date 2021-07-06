#version 460 core

#define PI 3.141592

layout(location = 0) out vec4 color;

uniform vec2 u_Resolution;

uniform int u_CircleCount;
uniform float u_Spacing;
uniform float u_Radius;
uniform vec4 u_Color;

uniform float u_RunTime;

vec4 circle(vec2 uv, vec2 pos, float rad, vec3 color) {
	float d = length(pos - uv) - rad;
	float t = clamp(d, 0.0, 1.0);
	return vec4(color, 1.0 - t);
}

void main() {

	vec2 uv = gl_FragCoord.xy;
	vec2 center = u_Resolution * 0.5;
    float offset = float(u_CircleCount) / 2.f + u_Spacing * (float(u_CircleCount) / 2.f) - u_Spacing / 2.f;
    
    // Background layer
	vec4 layer1 = vec4(0);
	
	// Circle
    vec4 circlec = vec4(u_Color.xyz, 0);
    
    for (int x = 0; x < u_CircleCount; x++) {
        float phase_shift = float(x) / (float(u_CircleCount) / 2.f) * PI;
        vec4 ncircle = circle(uv, vec2(center.x + (u_Spacing * float(x)) - offset, center.y + sin(u_RunTime * 5.f + phase_shift) * 3.f), u_Radius, u_Color.xyz);
        circlec = vec4(u_Color.xyz, circlec.a + ncircle.a);
    }
    
	// Blend the two
	color = mix(layer1, circlec, circlec.a);
    
    if (color.a < 0)
        discard;
}