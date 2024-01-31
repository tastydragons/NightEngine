#shader vertex
#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TextureIndex;

uniform mat4 u_ViewProjection;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TextureIndex;

void main()
{
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
	v_TextureIndex = a_TextureIndex;
	gl_Position = u_ViewProjection * vec4(position, 1.0);
};

#shader fragment
#version 450 core

layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TextureIndex;

uniform sampler2D u_Texture[32];

void main()
{
	color = texture(u_Texture[int(v_TextureIndex)], v_TexCoord ) * v_Color;
};