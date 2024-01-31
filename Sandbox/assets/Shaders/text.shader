#shader vertex
#version 450 core

layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>

out vec2 TexCoords;

uniform mat4  u_ViewProjection;

void main()
{
    gl_Position = u_ViewProjection * vec4(vertex.xy, 0.1, 1.0);
    TexCoords = vertex.zw;
} 

#shader fragment
#version 450 core

in vec2 TexCoords;

out vec4 color;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    color = vec4(textColor, 1.0) * sampled;
}  