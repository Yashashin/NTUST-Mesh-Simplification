#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <utility>


#include "Material.h"



class Object {
public:
	Object(std::string name,
		std::vector<glm::vec3> vertices);
	Object();

	void Draw(GLuint program,glm::vec3 viewPos);
	void DrawToPicking(GLuint program);
	void SetModelMatrix(glm::mat4 matrix);
	void SetProjectMatrix(glm::mat4 matrix);
	void SetViewMatrix(glm::mat4 matrix);
	glm::mat4 GetModelMatrix();
	glm::mat4 GetProjectMatrix();
	glm::mat4 GetViewMatrix();


public:
	std::string name;
	std::vector<glm::vec3> vertices;

	GLuint vao;
	GLuint vbo;

	glm::mat4 model_matrix;
	glm::mat4 project_matrix;
	glm::mat4 view_matrix;

};