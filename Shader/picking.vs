#version 430 core

layout(location = 0) in vec3 vPosition;


uniform mat4 View;
uniform mat4 Projection;
uniform mat4 Model;


void main()
{
	gl_Position = Projection*View*Model*vec4(vPosition, 1.0);
}
