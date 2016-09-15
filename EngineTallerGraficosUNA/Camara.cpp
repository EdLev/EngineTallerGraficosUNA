#include "Camara.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ContextoGLFW.h"

Camara::Camara(class ContextoGLFW& contexto) :
	MagnitudVelocidad(1.5f),
	Posicion(10.0f, 10.0f, -100.0f),
	Rotacion(0.0f),
	Velocidad(0.0f)
{
	contexto.RegistrarInterfazEntrada(this);
	Proyeccion = glm::perspective(45.0f, contexto.Ancho / (float)contexto.Alto, 0.1f, 1000.0f);
}

bool Camara::ManejarTecla(int tecla, int codigo, int accion, int modificadores)
{
	if (accion == GLFW_PRESS)
	{
		if (tecla == GLFW_KEY_W)
		{
			Velocidad.z += MagnitudVelocidad;
		}
		if (tecla == GLFW_KEY_A)
		{
			Velocidad.x += MagnitudVelocidad;
		}
		if (tecla == GLFW_KEY_S)
		{
			Velocidad.z -= MagnitudVelocidad;
		}
		if (tecla == GLFW_KEY_D)
		{
			Velocidad.x -= MagnitudVelocidad;
		}
		if (tecla == GLFW_KEY_R)
		{
			Velocidad.y -= MagnitudVelocidad;
		}
		if (tecla == GLFW_KEY_F)
		{
			Velocidad.y += MagnitudVelocidad;
		}
	}
	else if (accion == GLFW_RELEASE)
	{
		if (tecla == GLFW_KEY_W)
		{
			Velocidad.z -= MagnitudVelocidad;
		}
		if (tecla == GLFW_KEY_A)
		{
			Velocidad.x -= MagnitudVelocidad;
		}
		if (tecla == GLFW_KEY_S)
		{
			Velocidad.z += MagnitudVelocidad;
		}
		if (tecla == GLFW_KEY_D)
		{
			Velocidad.x += MagnitudVelocidad;
		}
		if (tecla == GLFW_KEY_R)
		{
			Velocidad.y += MagnitudVelocidad;
		}
		if (tecla == GLFW_KEY_F)
		{
			Velocidad.y -= MagnitudVelocidad;
		}
	}

	return false;
}

bool Camara::ManejarPosicionCursor(double x, double y)
{
	Rotacion.x += (float)x / 1000.0f;
	Rotacion.y += (float)y / 1000.0f;
	return false;
}

void Camara::AplicarMovimiento()
{
	Posicion += glm::vec3(glm::vec4(Velocidad, 0.0f) * Vista);
}

void Camara::ConstruirMatrizVista()
{
	Vista = glm::mat4(1.0f);
	Vista = glm::rotate(Vista, Rotacion.y, glm::vec3(1.0f, 0.0f, 0.0f));
	Vista = glm::rotate(Vista, Rotacion.x, glm::vec3(0.0f, 1.0f, 0.0f));
	Vista = glm::translate(Vista, Posicion);
}

void Camara::AplicarMatrices(GLuint ubicacionPrograma)
{
	// Asociar matrices de la cámara a los uniforms en el shader
	GLuint ubicacionVista = glGetUniformLocation(ubicacionPrograma, "Vista");
	glUniformMatrix4fv(ubicacionVista, 1, GL_FALSE, glm::value_ptr(Vista));
	GLuint ubicacionProy = glGetUniformLocation(ubicacionPrograma, "Proyeccion");
	glUniformMatrix4fv(ubicacionProy, 1, GL_FALSE, glm::value_ptr(Proyeccion));
}
