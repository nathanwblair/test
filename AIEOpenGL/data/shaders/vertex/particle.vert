#version 410

in vec4 Position;
in vec4 Colour;
out vec4 colour;

uniform mat4 projectionView;

void main() 
{
    colour = Colour;
    gl_Position = projectionView * Position;
}