#version 330

layout(location=0) in vec4 Position;
layout(location=2) in vec4 Normal;
layout(location=3) in vec4 Tangent;
layout(location=4) in vec4 Binormal;

layout(location=7) in vec2 TexCoords;

out vec4 vPosition;
out vec4 vNormal;
out vec4 vTangent;
out vec4 vBinormal;

out vec2 vTexCoords;

uniform mat4 ProjectionView;
uniform mat4 Model;

void main()
{
	gl_Position = ProjectionView * Model * Position;
	
	vNormal = Model * Normal;
	vTangent = Model * Tangent;
	vBinormal = Model * Binormal;
	
	vPosition = Model * Position;
	
	vTexCoords = TexCoords;
}