#version 330 core

in vec2 v_TexCoord;
in vec4 v_Color;
flat in uint v_TextureIndex;

out vec4 FragColor;

void main()
{
    FragColor = v_Color;
}
