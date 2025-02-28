#version 330 core // specify the version of GLSL

// uniform data
uniform mat4 uProjectionMatrix; // projection matrix
uniform mat4 uModelViewMatrix;	// model to view matrix

// mesh data
layout(location = 0) in vec3 aPosition; // vertex position from Obj
layout(location = 1) in vec3 aNormal;	// vertex normal from Obj

// model data (this must match the input of the vertex shader)
out VertexData {
	vec3 position;
	vec3 normal;
} v_out;


void main() {
	// transform vertex data to viewspace
	v_out.position = (uModelViewMatrix * vec4(aPosition, 1)).xyz;
	v_out.normal = normalize((uModelViewMatrix * vec4(aNormal, 0)).xyz);

	// set the screenspace position (needed for converting to fragment data)
	gl_Position = uProjectionMatrix * uModelViewMatrix * vec4(aPosition, 1);
}