#version 410

layout(location=0) in vec4 Position;
layout(location=1) in vec4 Colour;
layout(location=2) in vec2 TexCoord; 

out vec4 vColour;
out vec2 vTexCoord;

uniform mat4 ProjectionView;
uniform float time;
uniform float heightScale;



void main() 
{ 
	vTexCoord = TexCoord;

	vColour = Colour;
	vec4 P = Position;
	P.y += sin( time + Position.x ) * heightScale;
	gl_Position = ProjectionView * P;	
}