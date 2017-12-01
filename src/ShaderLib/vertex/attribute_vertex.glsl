#version 430 core

layout (location = 0)in vec3 position;
layout (location = 1)in vec3 normal;
layout (location = 2)in vec2 texCoord;
layout (location = 3)in vec3 color;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

