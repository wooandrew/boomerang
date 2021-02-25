#version 460 core

layout(location = 0) out vec4 color;

in vec2 v_Resolution;
in vec3 v_CamPos;
in float v_CellSize;

// TODO: make these variables in
float lineWidth      = 1.f;			// The width of each line (in pixels).
float darkenMultiple = 5.f;			// Multiple of which lines are darkened (e.g. every 5th line).

void main() {

	float width = lineWidth / v_CellSize;

    //vec2 uv = vec2(gl_FragCoord.x + v_CellSize / 2, gl_FragCoord.y / v_Resolution.y + (mod(v_Resolution.y, v_CellSize) - v_CellSize) / 2);
	vec2 uv = vec2(gl_FragCoord.x, gl_FragCoord.y / v_Resolution.y + v_Resolution.y / 2 - v_CellSize / 2);

	uv.x *= 1.0 / v_CellSize;
	uv.y /= v_CellSize;

    // Adjust the origin of each line thicker than one pixel to its center.
	uv.xy += (lineWidth > 1) ? width * 0.5 : width;
	
	// Make the grid resolution independent by offsetting the Y coordinates based on the current viewport height.
	uv.y -= gl_FragCoord.y / v_CellSize;

	// Apply camera translation.
	uv.x += v_CamPos.x / v_CellSize;
	uv.y -= v_CamPos.y / v_CellSize;
	
	float grid = 0.0;
	grid = max(step(fract(uv.x), width), grid); // X lines (horizontal)
	grid = max(step(fract(uv.y), width), grid); // Y lines (vertical)
	
	vec4 col = vec4(grid);
	
	if (col.r != 0.0 && col.g != 0.0 && col.b != 0.0) {
	
		float mx = mod(uv.x, float(darkenMultiple));
		float my = mod(uv.y, float(darkenMultiple));
		
		// Make lines that are a multiple of `darkenMultiple` darker than the rest.
		col.a = (mx < width || my < width) ? 0.2 : 0.15;
	} else {
		// Turn all pixels that aren't apart of the grid invisible as to not cover the rest of the scene.
		col.a = 0.0;
	}
	
	color = col;
}
