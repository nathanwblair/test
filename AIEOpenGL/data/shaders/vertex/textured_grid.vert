#version 410

layout(location=0) in vec4 Position;
layout(location=1) in vec4 Colour;
layout(location=2) in vec2 TexCoord; 

out vec4 vColour;
out vec2 vTexCoord;

uniform mat4 ProjectionView;
uniform float time;
uniform float heightScale;

uniform sampler2D perlinTexture;


void main() 
{ 
	vTexCoord = TexCoord;
	vColour = Colour;

	vec4 pos = Position;

	pos.y += texture(perlinTexture, TexCoord).r * 5;
	gl_Position = ProjectionView * pos;	
}