#version 460 core

layout(location = 0) out vec4 color;

uniform vec2 u_Resolution;
uniform vec3 u_CameraPosition;
uniform vec4 u_Color;
uniform float u_CellSize;

// TODO: make these variables in
float lineWidth      = 1.f;			// The width of each line (in pixels).
float darkenMultiple = 8.f;			// Multiple of which lines are darkened (e.g. every 5th line). Represents chunks.

void main() {

	float width = lineWidth / u_CellSize;

	vec2 uv = vec2(gl_FragCoord.x - (u_Resolution.x / 2.f) - (u_CellSize * (darkenMultiple / 2)), 
				   gl_FragCoord.y - (u_Resolution.y / 2.f) - (u_CellSize * (darkenMultiple / 2)));

	uv /= u_CellSize;

    // Adjust the origin of each line thicker than one pixel to its center.
	uv.xy += (lineWidth > 1) ? width * 0.5 : width;

	// Apply camera translation.
	uv.xy += u_CameraPosition.xy / u_CellSize;
	
	float grid = 0.0;
	grid = max(step(fract(uv.x), width), grid); // X lines (horizontal)
	grid = max(step(fract(uv.y), width), grid); // Y lines (vertical)
	
	vec4 col = vec4(grid);
	
	if (col.r != 0.0 && col.g != 0.0 && col.b != 0.0) {
	
		float mx = mod(uv.x, float(darkenMultiple));
		float my = mod(uv.y, float(darkenMultiple));
		
		// Make lines that are a multiple of `darkenMultiple` darker than the rest.
		col.a = (mx < width || my < width) ? 0.4 : 0.15;
	} else {
		// Discard all pixels not part of the grid.
		discard;
	}
	
	color = col * u_Color;
}
