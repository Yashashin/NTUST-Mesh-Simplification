#include"TextureImage.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

TextureImage::TextureImage()
{
	this->width = 0;
	this->height = 0;
	this->data = 0;
}

void TextureImage::Load_png(const char* path)
{
	int n;
	stbi_uc* data = stbi_load(path, &this->width, &this->height, &n, 4);
	if (data != NULL)
	{
		this->data = new unsigned char[this->width * this->height * 4 * sizeof(unsigned char)];
		memcpy(this->data, data, this->width * this->height * 4 * sizeof(unsigned char));
		stbi_image_free(data);

		//Generate empty texture
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, this->width,this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->data);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
	{
		printf("Image not loaded");
	}
}
