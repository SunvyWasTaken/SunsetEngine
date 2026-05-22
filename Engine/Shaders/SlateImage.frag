#version 330

in vec2 u_Uv;

uniform sampler2D U_Image;

out vec4 FragColor;

void main()
{
    FragColor = texture(U_Image, u_Uv);
    //FragColor = vec4(1, 0, 0, 1);
}