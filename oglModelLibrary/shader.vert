#version 450

in vec4 vVertex;
in vec3 vNormal;

uniform vec4 diffuseColor;
uniform vec3 vLightPosition;

uniform mat4 mvpMatrix;
uniform mat4 mvMatrix;
uniform mat3 normalMatrix;

// Color to fragment program
smooth out vec4 vVaryingColor;

void main(void)
{
	//vec4 diffuseColor = vec4(0.0, 0.0, 1.0, 1.0);

	//Get surface normal in eye coordinates
	vec3 vEyeNormal = normalMatrix * vNormal;

	// Get vertex position in eye coordinates
	vec4 vPosition4 = mvMatrix * vVertex;
	vec3 vPosition3 = vPosition4.xyz / vPosition4.w;

	// Get vector to light source
	vec3 vLightDir = normalize(vLightPosition - vPosition3);

	// Dot product gives us diffuse intensity
	float diff = max(0.0, dot(vEyeNormal, vLightDir));

	// Multiply intensity by diffuse color, force alpha to 1.0
	vVaryingColor.rgb = diff * diffuseColor.rgb;
	vVaryingColor.a = 1.0;

	// Let's not forget to transform the geometry
	gl_Position = mvpMatrix * vVertex;
}