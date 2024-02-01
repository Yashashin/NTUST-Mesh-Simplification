#pragma once

#include <string>
#include <vector>
#include <map>
#include <mutex>
//Include glew
#include <gl/glew.h>
#include <gl/gl.h>
#include <gl/glu.h>
//Include glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class ParamGL
{
public:

	// Shader program
	GLuint wireframeShader;
	GLuint pickingShader;
	GLuint modelShader;

	GLuint simpleTextureShader;
	GLuint uvMapShader;
	//Projection Matrix
	glm::mat4 projection;

	//View Matrix
	glm::mat4 view;

	//Model Matrix
	glm::mat4 model;

	//custom Shader
	GLuint postEffectInversionShader;

};
