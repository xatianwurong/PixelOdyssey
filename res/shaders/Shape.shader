#shader vertex
#version 330 core

layout(location = 0) in vec2 a_Position;

uniform mat4 u_Model;

void main()
{
    gl_Position = u_Model * vec4(a_Position, 0.0, 1.0);
}

#shader fragment
#version 330 core

uniform vec4 u_Color;
out vec4 color;

void main()
{
    color = u_Color;
}
