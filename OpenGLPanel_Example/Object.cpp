#include "Object.h"

Object::Object(std::string name, std::vector<glm::vec3> vertices)
{
	this->name = name;
	this->vertices = vertices;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
}

Object::Object()
{
	this->model_matrix = glm::mat4(1);
}

void Object::Draw(GLuint program,glm::vec3 viewPos)
{
	glUseProgram(program);
	
	glUniformMatrix4fv(
		glGetUniformLocation(program, "Model"), 1, GL_FALSE, &model_matrix[0][0]);
	glUniformMatrix4fv(
		glGetUniformLocation(program, "Projection"), 1, GL_FALSE, &project_matrix[0][0]);
	glUniformMatrix4fv(
		glGetUniformLocation(program, "View"), 1, GL_FALSE, &view_matrix[0][0]);

	glUniform3f(glGetUniformLocation(program, "viewPos"),viewPos.x,viewPos.y,viewPos.z);
	glUniform4f(glGetUniformLocation(program, "faceColor"), 1,1,0,1);
	glUniform4f(glGetUniformLocation(program, "wireColor"),0, 105.0/255.0,148.0/255.0,1);

	glBindVertexArray(this->vao);
	glDrawArrays(GL_TRIANGLES,0,this->vertices.size());
	glBindVertexArray(0);
	
}

void Object::DrawToPicking(GLuint program)
{
	glUseProgram(program);

	glUniformMatrix4fv(
		glGetUniformLocation(program, "Model"), 1, GL_FALSE, &model_matrix[0][0]);
	glUniformMatrix4fv(
		glGetUniformLocation(program, "Projection"), 1, GL_FALSE, &project_matrix[0][0]);
	glUniformMatrix4fv(
		glGetUniformLocation(program, "View"), 1, GL_FALSE, &view_matrix[0][0]);


	glBindVertexArray(this->vao);
	glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());
	glBindVertexArray(0);

}

void Object::SetModelMatrix(glm::mat4 matrix)
{
	this->model_matrix = matrix;
}

void Object::SetProjectMatrix(glm::mat4 matrix)
{
	this->project_matrix = matrix;
}

void Object::SetViewMatrix(glm::mat4 matrix)
{
	this->view_matrix = matrix;
}

glm::mat4 Object::GetModelMatrix()
{
	return this->model_matrix;
}

glm::mat4 Object::GetProjectMatrix()
{
	return this->project_matrix;
}

glm::mat4 Object::GetViewMatrix()
{
	return this->view_matrix;
}
