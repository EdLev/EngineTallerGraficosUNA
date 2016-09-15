#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ContextoGLFW.h"
#include "Modelo.h"
#include "Shader.h"
#include "Camara.h"

struct VerticeEjes
{
	glm::vec3 Posicion;
	glm::vec3 Color;
};

int main(int argc, char** argv)
{
	ContextoGLFW contexto;

	// Configurar cámara
	Camara camara(contexto);

	// Configurar modelos
	Shader shaderModelo("Modelo.vs", "Modelo.fs");
	Modelo mario("../Modelos/MarioLuigi/mario_obj.obj", shaderModelo, "../Modelos/MarioLuigi/textures/marioD.jpg", "../Modelos/MarioLuigi/textures/marioD.jpg" );
	Modelo dino("../Modelos/Dino/Full.obj", shaderModelo, "../Modelos/Dino/Diffuse 4096x4096.jpg", "../Modelos/Dino/Normal 4096x4096.jpg");

	// Configurar dibujo de ejes
	Shader shaderEjes("Ejes.vs", "Ejes.fs");
	const float tamañoEjes = 100.0f;
	VerticeEjes ejes[] = 
	{
		{{0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
		{{ tamañoEjes, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},

		{{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
		{{0.0f, tamañoEjes, 0.0f}, {0.0f, 1.0f, 0.0f}},

		{{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
		{{0.0f, 0.0f, tamañoEjes}, {0.0f, 0.0f, 1.0f}},
	};

	// Configurar estado de OpenGL
	// Activar prueba de profundidad
	glEnable(GL_DEPTH_TEST);

	GLuint VAOEjes;
	glCreateVertexArrays(1, &VAOEjes);
	glBindVertexArray(VAOEjes);
	GLuint VBOejes;
	glCreateBuffers(1, &VBOejes);
	glBindBuffer(GL_ARRAY_BUFFER, VBOejes);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ejes), ejes, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VerticeEjes), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VerticeEjes), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	GLenum error = glGetError();

	while (!contexto.VentanaFueCerrada())
	{
		contexto.ManejarEventos();

		// Simular
		camara.AplicarMovimiento();
		camara.ConstruirMatrizVista();

		// Limpiar pantalla
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Dibujar ejes
		glUseProgram(shaderEjes.Programa);
		camara.AplicarMatrices(shaderEjes.Programa);
		glBindVertexArray(VAOEjes);
		glDrawArrays(GL_LINES, 0, 6);
		glBindVertexArray(0);

		error = glGetError();

		// Dibujar modelos
		glUseProgram(shaderModelo.Programa);
		camara.AplicarMatrices(shaderModelo.Programa);
		mario.Dibujar();
		dino.Dibujar();

		error = glGetError();
		// 
		contexto.TerminarCuadro();
	}

	return 0;
	// La destrucción del contexto garantiza que GLFW se termine correctamente
}