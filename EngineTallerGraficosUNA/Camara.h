// Esta clase contiene la informaci�n para configurar la vista en OpenGL, y responde al control del usuario
#pragma once

#include <glm/glm.hpp>

#include "ContextoGLFW.h"
#include "InterfazEntrada.h"

class Camara : public InterfazEntrada
{
public:
	Camara(class ContextoGLFW& contexto);

	virtual bool ManejarTecla(int tecla, int codigo, int accion, int modificadores) override;
	virtual bool ManejarPosicionCursor(double x, double y) override;

	void AplicarMovimiento();
	void AplicarMatrices(GLuint ubicacionPrograma);

private:

	float MagnitudVelocidad;
	glm::vec3 Velocidad;
	glm::mat4 Vista;
	glm::mat4 Proyeccion;
};