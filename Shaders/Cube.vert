#version 330 core

layout(location = 0) in vec3 vData;
layout(location = 1) in vec3 vNormal;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec3 normal;

void main()
{
    gl_Position = projection * view * model * vec4(vData, 1.0);

    mat3 normalMatrix = transpose(inverse(mat3(model)));
    normal = normalMatrix * vNormal;
}