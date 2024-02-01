#pragma once
//Include glew
#include <gl/glew.h>
#include <gl/gl.h>
#include <gl/glu.h>
//Include glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class PickingTexture
{
public:
	PickingTexture();
	~PickingTexture();

	bool Init(int width, int height);
	void Enable();
	void Disable();
	GLuint ReadTexture(unsigned int x, unsigned int y);
	GLuint GetColorTex() { return fboColor; }

private:
	GLuint fbo;
	GLuint fboColor;
	GLuint fboDepth;

	void Clear();

};

