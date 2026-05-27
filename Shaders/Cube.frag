#version 330 core

in vec3 normal;

out vec4 FragColor;

void main()
{
    vec3 N = normalize(normal);

    vec3 lightDir = normalize(vec3(0.4, 1.0, 0.2));

    float diff = max(dot(N, lightDir), 0.0);

    vec3 baseColor = vec3(1.0);

    vec3 color = baseColor * (0.2 + diff);
    // 0.2 = lumière ambiante minimale

    FragColor = vec4(color, 1.0);
}