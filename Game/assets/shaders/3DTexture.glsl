// Basic Texture Shader

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_UV;
layout(location = 2) in float a_TexIndex;
layout(location = 3) in int a_EntityID;

layout(location = 0) out vec2 v_UV;
layout(location = 1) out flat float v_TexIndex;
layout(location = 2) out flat int v_EntityID;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjection;
};


void main()
{
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
	v_UV = a_UV;
	v_TexIndex = a_TexIndex;
	v_EntityID = a_EntityID;
}

#type fragment
#version 450 core

layout(location = 0) out vec4 color;
layout(location = 1) out int color2;

layout(location = 0) in vec2 v_UV;
layout(location = 1) in flat float v_TexIndex;
layout(location = 2) in flat int v_EntityID;

layout(binding = 0) uniform sampler2D u_Textures[32];

void main()
{
	//color = vec4(0.5, 0.5, 0.5, 1);
	if (v_TexIndex >= 32)
	{
		color = texture(u_Textures[0], v_UV);
	}
	else
	{
		color = texture(u_Textures[int(v_TexIndex)], v_UV);
	}

	color2 = v_EntityID;
}