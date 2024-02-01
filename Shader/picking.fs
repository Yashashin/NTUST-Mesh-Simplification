#version 430 core

out uint fragColor;

void main(void) 
{
	fragColor = gl_PrimitiveID + 1;
}