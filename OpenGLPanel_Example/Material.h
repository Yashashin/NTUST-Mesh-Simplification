#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include "Texture.h"

////Include glew
//#include <gl/glew.h>
//#include <gl/gl.h>
//#include <gl/glu.h>
////Include glm
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>

class Material
{
public:
	Material(std::string name, glm::vec3 Ka, glm::vec3 Kd, glm::vec3 Ks, std::string texture) {
		this->name = name;
		this->Ka = Ka;
		this->Kd = Kd;
		this->Ks = Ks;
		this->textureName = texture;
	}
	std::string name;
	glm::vec3 Ka;
	glm::vec3 Kd;
	glm::vec3 Ks;
	std::string textureName;
	Texture texture;
	
};