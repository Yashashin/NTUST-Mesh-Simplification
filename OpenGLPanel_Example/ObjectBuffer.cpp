#include "ObjectBuffer.h"



void ObjectBuffer::loadObj(const char* filePath,std::string name)
{
	if (name == "empty")
	{
		this->objects.push_back(Object());
		return;
	}
	std::cout << "Loading " << name << "..." << std::endl;
	std::vector<glm::vec3> v;
	std::vector <glm::ivec3> v_index;
	std::ifstream inStream;
	inStream.open(filePath, std::ios::in);
	if (!inStream.is_open())
	{
		std::cout << "Wrong path to read object" << std::endl;
		return;
	}
	std::string tmp;
	while (getline(inStream, tmp))
	{
	 
		std::istringstream sin(tmp);
		std::string type;
		int v0, v1, v2;
		GLfloat x, y, z;
		char slash;
		sin >> type;
		if (type == "v")
		{
			sin >> x >> y >> z;
			v.push_back(glm::vec3(x, y, z));
		}
		else if (type == "f")
		{
			sin >> v0 >> v1 >> v2;
			v_index.push_back(glm::ivec3(v0 - 1, v1 - 1, v2 - 1));
		}
	}
	std::vector<glm::vec3> points;
	for (int i = 0; i < v_index.size(); i++)
	{
		points.push_back(v[v_index[i].x]);
		points.push_back(v[v_index[i].y]);
		points.push_back(v[v_index[i].z]);
	}
	this->objects.push_back(Object(name, points));
	inStream.close();
}

void ObjectBuffer::loadTexture(const char* filePath,Material &material)
{
	material.texture.Load_png(filePath);
}

