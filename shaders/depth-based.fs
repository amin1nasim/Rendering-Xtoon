#version 330 core
out vec4 FragColour;

in vec3 FragPos;
in vec3 Normal;

uniform int r;
uniform float near;
uniform bool useLinear;
uniform vec3 lightPosition;
uniform sampler2D myTexture;


//====================================================
// The following function adapted from learnopengl.com
// https://learnopengl.com/Advanced-OpenGL/Depth-testing //
float LinearizeDepth(float depth, float far)
{
    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}
//====================================================

void main()
{
    float far = near * r;
    float depth = 0;
    if (useLinear){
      depth = LinearizeDepth(gl_FragCoord.z, far)/far; // divide by far to get depth in range [0,1] for visualization purposes
      depth = pow(depth, 6);
      depth *= far;
    }
    else{
      depth = gl_FragCoord.z;
    }

    // Light Direction (from fragment to light source)
    vec3 lightDir = normalize(lightPosition - FragPos);

    // Lambertian
    float L = dot(Normal, lightDir);
    // nomalzing between 0 and 1
    L = max(L, 0);

    // Depth-based attribute
    float D = min(1 - (log(depth/near)/log(far/near)), 1);

    // specifies texture coordinates
    vec2 TexCoord = vec2(L, D);

    FragColour = texture(myTexture, TexCoord);
}
