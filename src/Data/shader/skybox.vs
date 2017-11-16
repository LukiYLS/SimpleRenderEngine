
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal ;

uniform mat4 viewRotation;
uniform mat4 projection;
uniform float farPlane;
varying vec3 tex_coords;
void main()
{
	gl_Position = projection * viewRotation * vec4(position,1.0);
	tex_coords = position.xyz;
}