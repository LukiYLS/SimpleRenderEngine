#version 330 core

layout(location = 0)in vec3 position;
layout(location = 1)in vec2 tex;

out vec2 uv;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = model * vec4(position,1.0);
	gl_Position = view * gl_Position;
	gl_Position = projection * gl_Position;
	uv = tex;
}