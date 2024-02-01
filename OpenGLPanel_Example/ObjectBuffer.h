#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#include "Object.h"

class ObjectBuffer {
public:
	std::vector<Object> objects;
	void loadObj(const char* filePath, std::string name);
	void loadTexture(const char* filePath, Material& material);
};