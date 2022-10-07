// Basic Texture Shader

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjection;
};

void main()
{
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 color;

void main()
{
	color = vec4(0.1, 0.1, 0.1, 1);
}