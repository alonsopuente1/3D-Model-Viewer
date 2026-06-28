#pragma once

/*
Vertex.h

contains the struct definition for the Vertex, used for loading the models
*/

#include <glm/glm.hpp>

struct Vertex
{
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 tex;

	Vertex()
	{
		pos = glm::vec3(0.0f);
		normal = glm::vec3(0.0f);
		tex = glm::vec2(0.0f);
	}

	Vertex(glm::vec3 pos, glm::vec3 normal, glm::vec2 tex)
	{
		this->pos = pos;
		this->normal = normal;
		this->tex = tex;
	}
};