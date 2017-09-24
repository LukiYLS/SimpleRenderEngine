#version 330 core

in vec3 Position;
in vec3 Normal;
in vec2 Uv;
in vec4 lightSpacePos;

uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

float getShadow(vec4 lightspacePos)
{
	vec3 projectCoords = lightspacePos.xyz / lightspacePos.w;
	projectCoords *= 0.5 +0.5;
	
	float nearDepth = texture(shadowMap, projectCoords.xy).r;
	float currentDepth = projectCoords.z;
	
	return currentDepth > nearDepth ? 1.0 : 0.0; //1.0 means in the shadow
}

void main()
{
	
}