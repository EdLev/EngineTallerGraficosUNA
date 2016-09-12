#include "Camara.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ContextoGLFW.h"

Camara::Camara(class ContextoGLFW& contexto) :
	MagnitudVelocidad(1.0f),
	Velocidad(0.0f)
{
	contexto.RegistrarInterfazEntrada(this);
	Vista = glm::translate(Vista, glm::vec3(10.1f, 1.1f, -100.0f));
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
	return false;
}

void Camara::AplicarMovimiento()
{
	Vista = glm::translate(Vista, Velocidad);
}

void Camara::AplicarMatrices(GLuint ubicacionPrograma)
{
	GLuint ubicacionVista = glGetUniformLocation(ubicacionPrograma, "Vista");
	glUniformMatrix4fv(ubicacionVista, 1, GL_FALSE, glm::value_ptr(Vista));
	GLuint ubicacionProy = glGetUniformLocation(ubicacionPrograma, "Proyeccion");
	glUniformMatrix4fv(ubicacionProy, 1, GL_FALSE, glm::value_ptr(Proyeccion));
}
