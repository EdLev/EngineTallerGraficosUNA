#version 400 core

uniform mat4 Vista;
uniform mat4 Proyeccion;
uniform mat4 Modelo;

// Correspondiente a los campos de VerticeModelo
layout (location = 0) in vec3 Posicion;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 CoordTex;

out vec3 NormalVertice;
out vec2 CoordTexVertice;

void main()
{
	gl_Position = Proyeccion * Vista * Modelo * vec4(Posicion, 1.0f);
	NormalVertice = Normal;
	CoordTexVertice = CoordTex;
}