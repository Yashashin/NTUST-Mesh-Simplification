#version 430
layout (triangles) in;
layout (triangle_strip,max_vertices=3) out;


in VertexData{
 vec3 vPosition;
 vec3 vNormal;
 vec2 vTexCoord;
} vertexIn[];

out VertexData{
    vec3 gPosition;
    vec3 gNormal;
    vec2 gTexcoord;
    vec3 barycentric;
}vertexOut;


void main(void) 
{ 
    vertexOut.gPosition=vertexIn[0].vPosition;
    vertexOut.gNormal=vertexIn[0].vNormal;
    vertexOut.gTexcoord=vertexIn[0].vTexCoord;
    vertexOut.barycentric=vec3(1,0,0);
    gl_Position=gl_in[0].gl_Position;
    gl_PrimitiveID=gl_PrimitiveIDIn;
    EmitVertex();

    vertexOut.gPosition=vertexIn[1].vPosition;
    vertexOut.gNormal=vertexIn[1].vNormal;
    vertexOut.gTexcoord=vertexIn[1].vTexCoord;
    vertexOut.barycentric=vec3(0,1,0);
    gl_Position=gl_in[1].gl_Position;
    gl_PrimitiveID=gl_PrimitiveIDIn;
    EmitVertex();

    vertexOut.gPosition=vertexIn[2].vPosition;
    vertexOut.gNormal=vertexIn[2].vNormal;
    vertexOut.gTexcoord=vertexIn[2].vTexCoord;
    vertexOut.barycentric=vec3(0,0,1);
    gl_Position=gl_in[2].gl_Position;
    gl_PrimitiveID=gl_PrimitiveIDIn;
    EmitVertex();

    EndPrimitive();
    
}