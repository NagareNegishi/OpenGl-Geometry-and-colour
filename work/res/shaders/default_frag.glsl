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
//uniform vec3 uLightColor;

void main() {
	// calculate shading
	vec3 surfaceColor = fColor;  //vec3(0.066, 0.341, 0.215); old hard code
	
	
	//vec3 eye = normalize(-f_in.position); // direction towards the eye
	//float light = abs(dot(normalize(f_in.normal), eye)); // difference between the surface normal and direction towards the eye

		// normalize vectors
	vec3 normal = normalize(f_in.normal);
	vec3 lightDir = normalize(-uLightDirection);
	
	// calculate simple directional lighting
	float light = max(dot(normal, lightDir), 0.0);




	vec3 finalColor = mix(surfaceColor / 4, surfaceColor, light);

	// output to the frambuffer
	fb_color = vec4(finalColor, 1);
}
