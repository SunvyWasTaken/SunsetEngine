#version 330 core

in vec2 v_TexCoord;
in vec4 v_Color;
flat in uint v_TextureIndex;

out vec4 FragColor;

uniform sampler2DArray u_TextureArray;

void main()
{
    FragColor = texture(
            u_TextureArray,
            vec3(v_TexCoord, float(v_TextureIndex))
    ) * v_Color;
}
