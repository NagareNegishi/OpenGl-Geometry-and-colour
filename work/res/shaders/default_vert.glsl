#version 330 core

// uniform data
uniform mat4 uProjectionMatrix;
uniform mat4 uModelViewMatrix;

// color data
uniform vec3 uColor;

// mesh data
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;

// model data (this must match the input of the vertex shader)
out VertexData {
	vec3 position;
	vec3 normal;
} v_out;

// flag for color data
out vec3 fColor;

void main() {
	// transform vertex data to viewspace
	v_out.position = (uModelViewMatrix * vec4(aPosition, 1)).xyz;
	v_out.normal = normalize((uModelViewMatrix * vec4(aNormal, 0)).xyz);

	// set the screenspace position (needed for converting to fragment data)
	gl_Position = uProjectionMatrix * uModelViewMatrix * vec4(aPosition, 1);

	// set the color
	fColor = uColor;
}