#pragma once
//Include glew
#include <gl/glew.h>
#include <gl/gl.h>
#include <gl/glu.h>
//Include glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
class Texture {

public:
	Texture() {
		uvScale = 1;
		uvOffsetX = 0;
		uvOffsetY = 0;
		uvRotate = 0;
	}
	~Texture() {

	}

	int id;
	GLuint textureID;
	GLuint textureVbo;
	float uvRotate;
	float uvScale;
	float uvOffsetX;
	float uvOffsetY;
	std::vector<unsigned int> faceIDs;
	std::vector<unsigned int*> fvIDsPtr;
	std::vector<int> elementCount;

};