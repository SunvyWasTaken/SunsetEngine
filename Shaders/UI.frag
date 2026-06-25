#version 330 core

in vec2 v_TexCoord;
in vec4 v_Color;
in float v_TextureIndex;

uniform sampler2D u_Textures[16];

out vec4 FragColor;

vec4 SampleTexture(int textureIndex, vec2 texCoord)
{
    switch (textureIndex)
    {
        case 0: return texture(u_Textures[0], texCoord);
        case 1: return texture(u_Textures[1], texCoord);
        case 2: return texture(u_Textures[2], texCoord);
        case 3: return texture(u_Textures[3], texCoord);
        case 4: return texture(u_Textures[4], texCoord);
        case 5: return texture(u_Textures[5], texCoord);
        case 6: return texture(u_Textures[6], texCoord);
        case 7: return texture(u_Textures[7], texCoord);
        case 8: return texture(u_Textures[8], texCoord);
        case 9: return texture(u_Textures[9], texCoord);
        case 10: return texture(u_Textures[10], texCoord);
        case 11: return texture(u_Textures[11], texCoord);
        case 12: return texture(u_Textures[12], texCoord);
        case 13: return texture(u_Textures[13], texCoord);
        case 14: return texture(u_Textures[14], texCoord);
        case 15: return texture(u_Textures[15], texCoord);
        default: return vec4(1.0);
    }
}

void main()
{
    if (v_TextureIndex < 0.5)
    {
        FragColor = v_Color;
        return;
    }

    int textureIndex = int(v_TextureIndex + 0.5) - 1;
    FragColor = SampleTexture(textureIndex, v_TexCoord) * v_Color;
}
