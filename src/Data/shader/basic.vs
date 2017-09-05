#version 330 core

layout(location = 0)in vec3 position;
layout(location = 1)in vec3 normal;
layout(location = 2)in vec2 tex;


uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec2 uv;
out vec3 Position;
out vec3 Normal;
void main(){
	gl_Position = modelMatrix * vec4(position,1.0);
	Position = gl_Position.xyz;
	gl_Position = viewMatrix * gl_Position;	
	gl_Position = projectionMatrix * gl_Position;	
	Normal = normal;
	uv = tex;
}
