#version 430
in vec3 fPosition;
in vec3 fNormal;
in vec2 fTexCoord;


uniform vec4 faceColor = vec4(1,0,1,0);
uniform bool isTexture=false;

uniform sampler2D textureImage;

out vec4 fragColor;


void main(void)
{ 
    vec4 newFaceColor=faceColor;
    if(isTexture)
    {
        vec4 texColor=texture(textureImage,fTexCoord);
        newFaceColor = texColor;
    }
    vec3 viewDir=-fPosition;
    vec3 lightDir=vec3(0,0,-1);
    vec3 L=-lightDir;
    vec3 V=normalize(viewDir);
    vec3 N=normalize(fNormal);

    float ambient=0.01;
    float diffuse=max(dot(N,L),0);
    float specular=0;
    if(diffuse>0)
    {
        vec3 H=normalize(L+V);
        specular=pow(dot(N,H),128);
    } 
    newFaceColor=vec4(vec3(1.0)*(ambient+specular)+newFaceColor.xyz*diffuse,newFaceColor.a);

    // else
    // {
    //     fragColor=vec4(1,0,0,1);
    // }
    fragColor = newFaceColor;
  
}