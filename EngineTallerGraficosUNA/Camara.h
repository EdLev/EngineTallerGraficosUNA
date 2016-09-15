// Esta clase contiene la información para configurar la vista en OpenGL, y responde al control del usuario
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
	void ConstruirMatrizVista();
	void AplicarMatrices(GLuint ubicacionPrograma);

private:

	glm::vec3 Posicion;
	glm::vec2 Rotacion; // Solamente soportamos rotación sobre 2 ejes

	float MagnitudVelocidad;
	glm::vec3 Velocidad;


	glm::mat4 Vista;
	glm::mat4 Proyeccion;
};