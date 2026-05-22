#version 330

layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 uv;

uniform mat4 u_Proj;

out vec2 u_Uv;

void main()
{

    gl_Position = u_Proj * vec4(pos, 0.0, 1.0);
    // gl_Position = vec4((pos - vec2(640,360)) / 150.0, 0.0, 1.0);

    // vec2 tmp = vec2((pos.x - u_ScreenSize.x / 2 ) / 25.0, (pos.y - u_ScreenSize.y /2) / 25.0);

    // gl_Position = vec4(tmp, 0.0, 1.0);

    u_Uv = uv;
}