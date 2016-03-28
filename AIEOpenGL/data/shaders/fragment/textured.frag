#version 150

in vec4 vNormal;
in vec2 vTexCoords;

out vec4 fragColour;

uniform sampler2D DiffuseTexture;

void main()
{    
	fragColour = texture(DiffuseTexture, vTexCoords);
}