#include "ContextoGLFW.h"

#include <exception>
#include <iostream>
#include <IL/ilut.h>

#include "InterfazEntrada.h"

static ContextoGLFW* ContextoGlobal;

ContextoGLFW::ContextoGLFW()
{
	GLenum error;
	if (!glfwInit())
	{
		ManejarError(1, "Fallo al inicializar GLFW");
	}

	// Configurar manejo de errores
	glfwSetErrorCallback(ManejarError);

	// Crear la ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	Ventana = glfwCreateWindow(1280, 720, "Engine Taller UNA", nullptr, nullptr);
	if (Ventana == nullptr)
	{
		ManejarError(2, "Fallo al crear ventana");
	}
	glfwMakeContextCurrent(Ventana);

	// Inicializar la librería de manejo de extensiones de OpenGL (GLEW)
	glewExperimental = GL_TRUE;
	error = glewInit();
	if (error != GLEW_OK)
	{
		ManejarError(1, "Fallo al inicializar GLEW");
	}
	
	// Inicializar la librería para cargar imágenes (DevIL)
	ilInit();
	iluInit();
	ilutInit();

	// Inicializar el área para dibujar con OpenGL
	glfwGetFramebufferSize(Ventana, &Ancho, &Alto);
	glViewport(0, 0, Ancho, Alto);

	// Sincronizar los cuadros por segundo (FPS) de la aplicación con los de la pantalla 
	glfwSwapInterval(1);

	// Configurar entrada
	glfwSetKeyCallback(Ventana, ManejarTecla);
	glfwSetMouseButtonCallback(Ventana, ManejarClick);

	// Desactivar el cursor y capturar el mouse
	glfwSetInputMode(Ventana, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(Ventana, ManejarPosicionCursor);

	ContextoGlobal = this;
}

bool ContextoGLFW::VentanaFueCerrada()
{
	return glfwWindowShouldClose(Ventana) == GLFW_TRUE;
}

void ContextoGLFW::ManejarEventos()
{
	glfwPollEvents();
	glfwGetCursorPos(Ventana, &Cursor.X, &Cursor.Y);
	glfwSetCursorPos(Ventana, Ancho / 2.0f, Alto / 2.0f);
}

void ContextoGLFW::TerminarCuadro()
{
	glfwSwapBuffers(Ventana);
}

ContextoGLFW::~ContextoGLFW()
{
	glfwDestroyWindow(Ventana);
	glfwTerminate();
}

void ContextoGLFW::RegistrarInterfazEntrada(InterfazEntrada * Interfaz)
{
	InterfacesEntrada.push_back(Interfaz);
}

void ContextoGLFW::ManejarError(int error, const char * descripcion)
{
	std::cout << descripcion;
	throw(std::exception(descripcion));
}

void ContextoGLFW::ManejarTecla(GLFWwindow * ventana, int tecla, int codigo, int accion, int modificadores)
{
	bool consumido = false;
	if (accion == GLFW_PRESS)
	{
		if (tecla == GLFW_KEY_ESCAPE)
		{
			glfwSetWindowShouldClose(ventana, true);
			consumido = true;
		}
	}

	if (!consumido)
	{
		for (auto Interfaz : ContextoGlobal->InterfacesEntrada)
		{
			if (Interfaz->ManejarTecla(tecla, codigo, accion, modificadores))
			{
				break;
			}
		}
	}
}

void ContextoGLFW::ManejarClick(GLFWwindow * ventana, int boton, int accion, int modificadores)
{
	bool bConsumido = false;

	for (auto Interfaz : ContextoGlobal->InterfacesEntrada)
	{
		if (Interfaz->ManejarClick(boton, accion, modificadores))
		{
			break;
		}
	}
}

void ContextoGLFW::ManejarPosicionCursor(GLFWwindow * ventana, double x, double y)
{
	bool bConsumido = false;

	for (auto Interfaz : ContextoGlobal->InterfacesEntrada)
	{
		if (Interfaz->ManejarPosicionCursor(x - ContextoGlobal->Ancho / 2, y - ContextoGlobal->Alto / 2))
		{
			break;
		}
	}
}
