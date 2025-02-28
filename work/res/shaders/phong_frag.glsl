#version 330 core

// uniform data
uniform mat4 uProjectionMatrix;
uniform mat4 uModelViewMatrix;

// model color (from color picker)
uniform vec3 uColor;

// directional light data
uniform vec3 uLightDirection;
uniform vec3 uLightColor;

// phong model data
uniform float uAmbient;
uniform float uDiffuse;
uniform float uSpecular;
uniform float uShininess;

// viewspace data (this must match the output of the fragment shader)
in VertexData {
	vec3 position;
	vec3 normal;
} f_in;

// framebuffer output
out vec4 fb_color;

// use phong model to calculate color
void main() {
	//normalize the normal
	vec3 normal = normalize(f_in.normal);

	// calculate the view direction
	vec3 viewDir = normalize(-f_in.position);

	// calculate the light direction
	vec3 lightDir = normalize(-uLightDirection);

	// ambient
	vec3 ambient = uAmbient * uLightColor;

	// diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = uDiffuse * diff * uLightColor;

	// specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), uShininess);
	vec3 specular = uSpecular * spec * uLightColor;

	// final color
	vec3 finalColor = (ambient + diffuse + specular) * uColor;

	// output to the frambuffer
	fb_color = vec4(finalColor, 1);
}
