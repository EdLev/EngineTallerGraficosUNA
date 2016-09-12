#version 400 core

// Correspondiente a los campos de VerticeEjes

layout (location = 0) in vec3 Posicion;
layout (location = 1) in vec3 Color;

uniform mat4 Vista;
uniform mat4 Proyeccion;

out vec4 ColorVertice;

void main()
{
	gl_Position = Proyeccion * Vista * vec4(Posicion, 1.0f);
	ColorVertice = vec4(Color, 1.0f);
}