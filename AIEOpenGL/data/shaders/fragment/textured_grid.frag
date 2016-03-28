#version 410

in vec4 vColour;
in vec2 vTexCoord;

out vec4 FragColor;

uniform sampler2D diffuse;

void main()	
{ 
	FragColor = texture(diffuse, vTexCoord); 
}