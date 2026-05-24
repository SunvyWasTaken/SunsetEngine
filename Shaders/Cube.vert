#version 330 core

layout(location = 0) in vec3 vData;

uniform vec3 location;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    gl_Position = projection * view * vec4(vData, 1.0);
}