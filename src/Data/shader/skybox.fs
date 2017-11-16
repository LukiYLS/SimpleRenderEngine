#version 330 core

uniform samplerCube cubeMap;
varying vec3 tex_coords;

void main()
{
	vec3 rgb = textureCube(cubeMap, tex_coords).rgb;
	gl_FragColor = vec4(rgb,1.0);
}