#version 430
layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vTexcoord;

uniform mat4 View;
uniform mat4 Projection;
uniform mat4 Model;

uniform mat4 uvScale;
uniform mat4 uvRotate;
uniform mat4 uvOffset;

out vec3 fPosition;
out vec3 fNormal;
out vec2 fTexCoord;



void main(void) 
{ 
 mat4 MVP = Projection*View*Model;
    mat4 MV=View*Model;
    vec4 newTexcoord =uvOffset*uvRotate * uvScale* vec4(vTexcoord, 0.0, 1.0);
    fPosition=vec3(MV*vec4(vPosition,1));
 fTexCoord= newTexcoord.xy;
    fNormal=mat3(transpose(inverse(Model)))*vNormal;
    
    gl_Position = MVP * vec4(vPosition,1);
}