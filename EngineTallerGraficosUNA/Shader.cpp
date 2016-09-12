#include "Shader.h"

#include <iostream>

GLuint CargarShader(const std::string& nombreArchivo, GLuint tipo)
{
	// Copiar el texto del código a memoria
	std::ifstream archivo(nombreArchivo);
	std::string codigo;
	if (archivo)
	{
		archivo.seekg(0, std::ios::end);
		codigo.resize(archivo.tellg());
		archivo.seekg(0, std::ios::beg);
		archivo.read(&codigo.front(), codigo.size());
		archivo.close();
	}

	// Compilar el shader
	const char* c_str = codigo.c_str();
	GLuint shader = glCreateShader(tipo);
	glShaderSource(shader, 1, &c_str, nullptr);
	glCompileShader(shader);

	GLint estado;
	GLchar info[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &estado);
	if (estado == 0)
	{
		glGetShaderInfoLog(shader, 512, nullptr, info);
		std::cout << "Fallo al compilar shader " << nombreArchivo << "\n" << info;
	}

	return shader;
}

Shader::Shader(const std::string & archivoVertices, const std::string& archivoFragmentos)
{
	GLenum error = glGetError();
	// Compilar cada shader 
	GLuint shaderVertices = CargarShader(archivoVertices, GL_VERTEX_SHADER);
	GLuint shaderFragmentos = CargarShader(archivoFragmentos, GL_FRAGMENT_SHADER);

	// Unirlos en un programa
	Programa = glCreateProgram();
	glAttachShader(Programa, shaderVertices);
	glAttachShader(Programa, shaderFragmentos);
	glLinkProgram(Programa);

	// Podemos deshacernos de los shaders individuales cuando el programa está completo
	glDeleteShader(shaderVertices);
	glDeleteShader(shaderFragmentos);
}
