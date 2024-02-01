#version 410 core

out vec4 fragColor;

in VertexData
{
	vec2 texcoord;
} vertexData;

uniform sampler2D textureImage;

void main()
{
	fragColor = texture(textureImage, vertexData.texcoord);

}
