#version 330 core

in vec3 WorldPos;

out vec4 FragColor;

void main()
{
    vec3 dx = dFdx(WorldPos);
    vec3 dy = dFdy(WorldPos);

    vec3 normal = normalize(cross(dx, dy));

    vec3 lightDir = normalize(vec3(0.4, 1.0, 0.2));

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 baseColor = vec3(1.0);

    vec3 color = baseColor * (0.2 + diff);
    // 0.2 = lumière ambiante minimale

    FragColor = vec4(color, 1.0);
}