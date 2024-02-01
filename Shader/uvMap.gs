#version 430
layout (triangles) in;
layout (triangle_strip,max_vertices=3) out;



out VertexData{
    vec3 barycentric;
}vertexOut;


void main(void) 
{ 
    vertexOut.barycentric=vec3(1,0,0);
    gl_Position=gl_in[0].gl_Position;
    gl_PrimitiveID=gl_PrimitiveIDIn;
    EmitVertex();

    vertexOut.barycentric=vec3(0,1,0);
    gl_Position=gl_in[1].gl_Position;
    gl_PrimitiveID=gl_PrimitiveIDIn;
    EmitVertex();

    vertexOut.barycentric=vec3(0,0,1);
    gl_Position=gl_in[2].gl_Position;
    gl_PrimitiveID=gl_PrimitiveIDIn;
    EmitVertex();

    EndPrimitive();
    
}