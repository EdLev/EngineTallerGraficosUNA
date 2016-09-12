// Esta clase maneja la librería (GLFW) que usamos para manejar la ventana, interfaz con OpenGL y entrada de usuario (teclado/mouse)
#pragma once

#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct PosicionMouse
{
	double X;
	double Y;
};

class ContextoGLFW
{
public:

	ContextoGLFW();
	~ContextoGLFW();

	void RegistrarInterfazEntrada(class InterfazEntrada* Interfaz);

	bool VentanaFueCerrada();
	void ManejarEventos();
	void TerminarCuadro();

	int Ancho;
	int Alto;

	std::vector<class InterfazEntrada*> InterfacesEntrada;

private:

	GLFWwindow* Ventana;
	PosicionMouse Cursor;

	static void ManejarError(int error, const char* descripcion);
	static void ManejarTecla(GLFWwindow* ventana, int tecla, int codigo, int accion, int modificadores);
	static void ManejarClick(GLFWwindow* ventana, int boton, int accion, int modificadores);
	static void ManejarPosicionCursor(GLFWwindow* ventana, double x, double y);
};
