#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "ContextoGLFW.h"
#include "Shader.h"

struct VerticeModelo
{
	glm::vec3 Posicion;
	glm::vec3 Normal;
	glm::vec2 CoordTextura;

	bool operator<(const VerticeModelo& otro) const
	{
		return memcmp(this, &otro, sizeof(this)) == 1;
	}
};

class Modelo
{
public:

	Modelo(const std::string& nombreArchivo, const Shader& shader, const std::string& nombreTexturaDifusa, const std::string& nombreTexturaNormal);

	void Dibujar();

	glm::vec3 Posicion;
	glm::quat Rotacion;

	Shader Shader;

private:

	size_t NumIndices;
	GLuint VAO;
	GLuint TexturaDifusa;
	GLuint TexturaNormal;

};