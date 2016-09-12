#include "Modelo.h"

#include <fstream>
#include <sstream>
#include <map>
#include <cassert>

#include<glm/gtc/type_ptr.hpp>
#include <IL/ilut.h>

#include "ContextoGLFW.h"

void SepararLinea(const std::string& linea, const char delimitador, std::vector<std::string>& salida)
{
	std::istringstream stream(linea);
	while (stream)
	{
		std::string simbolo;
		std::getline(stream, simbolo, delimitador);
		salida.push_back(simbolo);
	}
}

glm::vec2 LeerVec2(std::vector<std::string>::const_iterator iter)
{
	glm::vec2 resultado;
	resultado.x = (float)atof((*iter++).c_str());
	resultado.y = (float)atof((*iter++).c_str());
	return resultado;
}

glm::vec3 LeerVec3(std::vector<std::string>::const_iterator iter)
{
	glm::vec3 resultado;
	resultado.x = (float)atof((*iter++).c_str());
	resultado.y = (float)atof((*iter++).c_str());
	resultado.z = (float)atof((*iter++).c_str());
	return resultado;
}

GLuint CargarTextura(const std::string& nombreArchivo)
{
	GLuint resultado = 0;

	if (!nombreArchivo.empty())
	{
		// Configurar DevIL
		ILuint imagen = ilGenImage();
		ilBindImage(imagen);
		ilEnable(IL_ORIGIN_SET);
		ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
		bool exito = ilLoadImage(nombreArchivo.c_str()) == 1;
		if (exito)
		{
			// Convertir a formato de OpenGL
			ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
			glGenTextures(1, &resultado);
			glBindTexture(GL_TEXTURE_2D, resultado);
			// Enviar imagen
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
				ilGetInteger(IL_IMAGE_WIDTH),
				ilGetInteger(IL_IMAGE_HEIGHT),
				0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());
			ilDeleteImage(imagen);
		}
	}

	return resultado;
}

Modelo::Modelo(const std::string & nombreOBJ, const ::Shader& shader, const std::string& nombreTexturaDifusa, const std::string& nombreTexturaNormal) :
	Shader(shader),
	TexturaDifusa(0),
	TexturaNormal(0)
{
	using namespace std;

	// Estos vectores son temporales porque se copian a la tarjeta de video, no es necesario mantenerlos en memoria
	vector<VerticeModelo> vertices;
	vector<uint32_t> indices;

	// El formato OBJ tiene cada parte de los vértices en secciones aparte - aquí los juntamos para formar VerticeModelo
	vector<glm::vec3> posiciones;
	vector<glm::vec3> normales;
	vector<glm::vec2> coordsTex;

	map<VerticeModelo, uint32_t> mapaVertices;

	ifstream archivo(nombreOBJ);
	while (archivo)
	{
		string linea;
		getline(archivo, linea);
		vector<string> simbolos;
		SepararLinea(linea, ' ', simbolos);

		if (!simbolos.empty())
		{
			auto iter = simbolos.begin();
			string tipo = (*iter++);

			// remover espacio extra del formato OBJ
			if (iter != simbolos.end() && iter->empty())
			{
				++iter;
			}

			if (tipo == "v")
			{
				posiciones.push_back(LeerVec3(iter));
			}
			else if (tipo == "vn")
			{
				normales.push_back(LeerVec3(iter));
			}
			else if (tipo == "vt")
			{
				coordsTex.push_back(LeerVec2(iter));
			}
			else if (tipo == "f")
			{
				// Leer los índices de cada polígono
				uint32_t numVertices = 0;
				for (; iter != simbolos.end(); ++iter)
				{
					// usar cada grupo de índices en el archivo para armar los vértices
					vector<string> textoIndices;
					string grupoIndices = *iter;
					SepararLinea(grupoIndices, '/', textoIndices);
					VerticeModelo verticeNuevo;
					// convertir de texto a índices numéricos, tomando en cuenta que OBJ empieza a contar de 1 en vez de 0
					vector<uint32_t> indicesCara;
					for (auto textoIndice : textoIndices)
					{
						if (!textoIndice.empty())
						{
							indicesCara.push_back(atoi(textoIndice.c_str()) - 1);
						}
					}

					if (indicesCara.size() >= 1)
					{
						verticeNuevo.Posicion = posiciones[indicesCara[0]];
						++numVertices;

						if (indicesCara.size() >= 2)
						{
							verticeNuevo.CoordTextura = coordsTex[indicesCara[1]];
						}
						if (indicesCara.size() >= 3)
						{
							verticeNuevo.Normal = normales[indicesCara[2]];
						}

						auto busqueda = mapaVertices.find(verticeNuevo);
						//if (busqueda == mapaVertices.end())
						{
							size_t indiceNuevo = vertices.size();
							vertices.push_back(verticeNuevo);
							indices.push_back(indiceNuevo);
							//mapaVertices[verticeNuevo] = indiceNuevo;
							++indiceNuevo;
						}
						//else
						//{
						//	indices.push_back(busqueda->second);
						//}
					}

				}

				// Covertir cuadrados a 2 triángulos
				if (numVertices == 4)
				{
					// Completar el segundo triángulo
					// 0 1 2 3 -> 0 1 2 0 2 3
					indices.insert(indices.end() - 1, *(indices.rbegin() + 3));
					indices.insert(indices.end() - 1, *(indices.rbegin() + 2));
				}
				if (numVertices > 4)
				{
					// No manejamos más de 4 vértices por cara
					assert(false);
				}
			}
		}
	}

	NumIndices = indices.size();

	GLenum error = glGetError();

	// VAO encapsula todos los datos que vamos a enviarle a OpenGL para este objeto
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	{
		// Copiar vértices a OpenGL
		GLuint bufferVertices;
		glGenBuffers(1, &bufferVertices);
		glBindBuffer(GL_ARRAY_BUFFER, bufferVertices);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices.front()), &vertices.front(), GL_STATIC_DRAW);
		error = glGetError();
		// Copiar índices a OpenGL
		GLuint bufferIndices;
		glGenBuffers(1, &bufferIndices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferIndices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices.front()), &indices.front(), GL_STATIC_DRAW);
		error = glGetError();
		// Describirle la forma de los datos a OpenGL
		// Cada llamada a glVertexAttribPointer le corresponde a uno de los campos de VerticeModelo
		// Posicion
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VerticeModelo), (GLvoid*)offsetof(VerticeModelo, Posicion));
		glEnableVertexAttribArray(0);
		// Normal
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VerticeModelo), (GLvoid*)offsetof(VerticeModelo, Normal));
		glEnableVertexAttribArray(1);
		// CoordTex
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VerticeModelo), (GLvoid*)offsetof(VerticeModelo, CoordTextura));
		glEnableVertexAttribArray(2);
		error = glGetError();
	}
	// Soltar el VAO
	glBindVertexArray(0);

	TexturaDifusa = CargarTextura(nombreTexturaDifusa);
	TexturaNormal = CargarTextura(nombreTexturaNormal);
}

void Modelo::Dibujar()
{
	// Asociar texturas con uniforms en el shader
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TexturaDifusa);
	glGenerateMipmap(GL_TEXTURE_2D);
	glUniform1i(glGetUniformLocation(Shader.Programa, "TexturaDifusa"), 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TexturaNormal);
	glUniform1i(glGetUniformLocation(Shader.Programa, "TexturaNormal"), 1);

	// Asociar matriz de modelo con uniform en el shader
	glUniformMatrix4fv(glGetUniformLocation(Shader.Programa, "Modelo"), 1, false, glm::value_ptr(ModeloAMundo));

	// Activar VAO
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, NumIndices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
