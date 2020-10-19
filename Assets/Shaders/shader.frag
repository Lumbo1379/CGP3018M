#version 440 core

uniform float uTime;
out vec4 colour;

void main()
{
	colour = vec4(float(sin(uTime)) * 0.5 + 0.5, float(cos(uTime)) * 0.5 + 0.5, float(cos(uTime)) + 0.5, 1.0);
}