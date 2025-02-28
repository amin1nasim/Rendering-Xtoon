#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec2 TexCoord;

uniform int r; // Exponent of N.V
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPosition;
uniform vec3 viewPos;


void main()
{
    // World coordinate
    vec3 FragPos = vec3(model * vec4(aPos, 1.0));

    // Normal
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 Normal = normalize(normalMatrix * aNormal);

    // Light Direction (from vetex to light source)
    vec3 lightDir = normalize(lightPosition - FragPos);

    // Lambertian
    float L = dot(Normal, lightDir);
    // nomalzing between 0 and 1
    L = max(L, 0);

    // Orientation-based attribute mapping
    vec3 viewDir = normalize(viewPos - FragPos);
    float D = pow(abs(dot(Normal, viewDir)), r);

    // Texture coordiante
    TexCoord = vec2(L, D);


    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
