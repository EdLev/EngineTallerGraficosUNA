// Esta clase provee una interfaz para clases que quieran manejar eventos de entrada (teclado/mouse)
#pragma once

class InterfazEntrada
{
public:

	virtual bool ManejarTecla(int tecla, int codigo, int accion, int modificadores) { return false; }
	virtual bool ManejarClick(int boton, int accion, int modificadores) { return false; }
	virtual bool ManejarPosicionCursor(double x, double y) { return false; }
};