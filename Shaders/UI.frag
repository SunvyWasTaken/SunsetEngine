#version 330 core

in vec2 v_TexCoord;
in vec4 v_Color;
flat in uint v_TextureIndex;

out vec4 FragColor;

uniform sampler2D u_Textures[16];

vec4 SampleTexture(uint textureIndex, vec2 texCoord)
{
    switch (textureIndex)
    {
        case  0u: return texture(u_Textures[0], texCoord);
        case  1u: return texture(u_Textures[1], texCoord);
        case  2u: return texture(u_Textures[2], texCoord);
        case  3u: return texture(u_Textures[3], texCoord);
        case  4u: return texture(u_Textures[4], texCoord);
        case  5u: return texture(u_Textures[5], texCoord);
        case  6u: return texture(u_Textures[6], texCoord);
        case  7u: return texture(u_Textures[7], texCoord);
        case  8u: return texture(u_Textures[8], texCoord);
        case  9u: return texture(u_Textures[9], texCoord);
        case 10u: return texture(u_Textures[10], texCoord);
        case 11u: return texture(u_Textures[11], texCoord);
        case 12u: return texture(u_Textures[12], texCoord);
        case 13u: return texture(u_Textures[13], texCoord);
        case 14u: return texture(u_Textures[14], texCoord);
        case 15u: return texture(u_Textures[15], texCoord);
        default: return vec4(1.0);
    }
}

void main()
{
    FragColor = SampleTexture(v_TextureIndex, v_TexCoord) * v_Color;
}
