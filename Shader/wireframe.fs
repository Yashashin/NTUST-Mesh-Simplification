
    #version 430

in VertexData{
 vec3 gPosition;
 vec3 gNormal;
 vec2 gTexcoord;
 vec3 barycentric; 
}vertexIn;

uniform vec4 faceColor;
uniform vec4 wireColor;
uniform bool isTexture;

uniform sampler2D textureImage;

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
 vec3 viewDir=-vertexIn.gPosition;
 vec3 lightDir=vec3(0,0,-1);
 vec3 L=-lightDir;
 vec3 V=normalize(viewDir);
 vec3 N=normalize(vertexIn.gNormal);

 float ambient=0.01;
 float diffuse=max(dot(N,L),0);
 float specular=0;
 if(diffuse>0)
 {
 vec3 H=normalize(L+V);
 specular=pow(dot(N,H),128);
 } 
 newFaceColor=vec4(vec3(1.0)*(ambient+specular)+newFaceColor.xyz*diffuse,newFaceColor.a);
 float ef=edgeFactor();
 if(isTexture)
 {
  vec4 color=texture(textureImage,vertexIn.gTexcoord);
  fragColor=vec4(vec3(1.0)*(ambient+specular)+color.xyz*diffuse,newFaceColor.a);
 }
 else
 {
  fragColor=mix(wireColor,newFaceColor,ef);
 }
}