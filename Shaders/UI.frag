#version 330 core

in vec2 v_TexCoord;
in vec4 v_Color;
flat in uint v_TextureIndex;

out vec4 FragColor;

uniform sampler2D u_Textures[16];

void main()
{
    FragColor = texture(u_Textures[int(v_TextureIndex)], v_TexCoord) * v_Color;
}
