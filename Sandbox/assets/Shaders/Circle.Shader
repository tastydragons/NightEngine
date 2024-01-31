#shader vertex
#version 450 core

layout(location = 0) in vec3 a_WorldPosition;
layout(location = 1) in vec3 a_LocalPosition;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in float a_Depth;
layout(location = 4) in float a_Decline;

uniform mat4 u_ViewProjection;

out vec4 v_Color;
out vec3 v_LocalPosition;
out float v_Depth;
out float v_Decline;

void main()
{
	v_Color = a_Color;
	v_LocalPosition = a_LocalPosition;
	v_Depth = a_Depth;
	v_Decline = a_Decline;
	gl_Position = u_ViewProjection * vec4(a_WorldPosition, 1.0);
};

#shader fragment
#version 450 core

layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec3 v_LocalPosition;
in float v_Depth;
in float v_Decline;

void main()
{
	float distance = 1.0 - length(v_LocalPosition);
	float circle = smoothstep(0.0, v_Decline, distance);
	circle *= smoothstep(v_Depth + v_Decline, v_Depth, distance);

	if (circle == 0.0)
		discard;
	
	color = v_Color;
	color.a *= circle;
};