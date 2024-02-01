#version 430
layout(location = 2) in vec2 vTexcoord;

uniform mat4 View;
uniform mat4 Projection;
uniform mat4 Model;



void main(void) 
{ 
    vec2 offset = vec2(-0.5, -0.5);
	vec4 pos = vec4(vTexcoord + offset, 0.0, 1.0);
	mat4 MVP = Projection*View*Model;
    
    gl_Position = MVP * pos;
}