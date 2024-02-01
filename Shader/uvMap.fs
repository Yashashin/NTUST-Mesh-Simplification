#version 430

in VertexData{
 vec3 barycentric;	
}vertexIn;

uniform vec4 faceColor;
uniform vec4 wireColor;

out vec4 fragColor;

float edgeFactor()
{
	vec3 d=fwidth(vertexIn.barycentric);
	vec3 a3 = smoothstep(vec3(0.0), d * 1.5, vertexIn.barycentric);
	return min(min(a3.x, a3.y), a3.z);
}
void main(void)
{ 
	vec4 newFaceColor=faceColor;
	float ef=edgeFactor();
	fragColor=mix(wireColor,newFaceColor,ef);
}
	
    