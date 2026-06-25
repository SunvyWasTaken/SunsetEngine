#version 330 core

in vec2 v_TexCoord;
in vec4 v_Color;

out vec4 FragColor;

void main()
{
//    FragColor = SampleTexture(v_TextureIndex, v_TexCoord) * v_Color;
    FragColor = v_Color;
}
