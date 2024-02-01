#version 410 core

uniform mat4 Model;
uniform mat4 Projection;
uniform mat4 View;

uniform mat4 uvScale;
uniform mat4 uvRotate;
uniform mat4 uvOffset;

out VertexData
{
	vec2 texcoord;
} vertexData;

const vec2[] points = vec2[4](vec2(-0.5, -0.5), vec2(-0.5, 0.5), vec2(0.5, -0.5), vec2(0.5, 0.5));
const vec2[] uv = vec2[4]( vec2(0, 0), vec2(0, 1),vec2(1, 0), vec2(1, 1));

void main()
{
	mat4 MVP=Projection*View*Model;
	gl_Position = MVP*inverse(uvOffset)*vec4(points[gl_VertexID], 0.0, 1.0);
	vec4 newTexcoord=uvOffset*uvRotate*uvScale*vec4(uv[gl_VertexID],0,0);
	vertexData.texcoord =newTexcoord.xy ;
}