#shader vertex
#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 a_Color;

uniform mat4 u_ViewProjection;

out vec4 v_Color;
void main()
{
	
	v_Color = a_Color;
	gl_Position = u_ViewProjection * vec4(position, 1.0);
	
};

#shader fragment
#version 450 core

in vec4 v_Color;
layout(location = 0) out vec4 color;

void main()
{
	
	// Get the normalized distance of the current fragment from the center of the screen
	vec2 pos = gl_FragCoord.xy / vec2(1920.0, 1080.0);
	float impact = max(abs(pos.x - 0.5), abs(pos.y - 0.5));
	impact = pow(impact, 0.5);
	
	// Create the red color with intensity based on the impact factor
	color = v_Color * impact;
 
};