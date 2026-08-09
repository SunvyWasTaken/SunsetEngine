#version 330 core

layout(location = 0) in vec2 data;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    gl_Position = projection * view * model * vec4(data.x, data.y, 0.0, 1.0);
}
