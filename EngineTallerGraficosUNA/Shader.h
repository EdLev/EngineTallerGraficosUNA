#pragma once

#include <string>
#include <fstream>

#include "ContextoGLFW.h"

class Shader
{
public:

	Shader(const std::string& archivoVertices, const std::string& archivoFragmentos);
	GLuint Programa;
};
