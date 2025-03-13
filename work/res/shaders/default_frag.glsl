#version 330 core

// uniform data
uniform mat4 uProjectionMatrix;
uniform mat4 uModelViewMatrix;

// viewspace data (this must match the output of the fragment shader)
in VertexData {
	vec3 position;
	vec3 normal;
} f_in;

// flag for color data
in vec3 fColor;

// framebuffer output
out vec4 fb_color;

// directional light data
uniform vec3 uLightDirection;

// calculate shading
void main() {
	vec3 surfaceColor = fColor; // input from color picker

	// calculate simple directional lighting
	vec3 normal = normalize(f_in.normal);
	vec3 lightDir = normalize(-uLightDirection);
	float light = max(dot(normal, lightDir), 0.0);

	// calculate final color
	vec3 finalColor = mix(surfaceColor / 4, surfaceColor, light);
	fb_color = vec4(finalColor, 1);
}
