#ifndef _IMAGE_H_
#define _IMAGE_H_
//Include glew
#include <gl/glew.h>
#include <gl/gl.h>
#include <gl/glu.h>
//Include glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//custom
#include <iostream>
#include <vector>

class TextureImage
{
public :
	TextureImage();
	void Load_png(const char* path);
	int width;
	int height;
	unsigned char* data;
	GLuint textureID;
};


#endif

