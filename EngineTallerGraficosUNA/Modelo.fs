#version 400 core

uniform sampler2D TexturaDifusa;
uniform sampler2D TexturaNormal;

in vec3 NormalVertice;
in vec2 CoordTexVertice;

out vec4 Color;

void main()
{
	vec3 dirLuz = normalize(vec3(1.0f, 1.0f, 1.0f));
	//Color = vec4(1.0f, 1.0f, 0.0f, 1.0f) * dot(NormalVertice, dirLuz);
	//Color = vec4(NormalVertice,1.0f);
	//Color = vec4(1.0f, 1.0f, 0.5f, 1.0f);
	Color = texture(TexturaDifusa, CoordTexVertice);// * dot(NormalVertice, dirLuz);
	//Color = vec4(CoordTexVertice, 0.0f, 1.0f);
	Color = texture(TexturaNormal, CoordTexVertice);
}