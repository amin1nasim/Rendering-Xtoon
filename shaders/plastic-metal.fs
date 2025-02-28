#version 330 core

out vec4 FragColour;

in vec2 TexCoord;

//texture samplers
uniform sampler2D myTexture;

void main()
{
    FragColour = texture(myTexture, TexCoord);
}
