#include <cstdio>
#include <cstdlib>
#include <iostream>

//include shape, shader header files
#include "Triangle.h"
#include "ShaderClass.h"
#include "KeyboardHandler.h"

// // GLEW - OpenGL Extension Wrangler - http://glew.sourceforge.net/
// // NOTE: include before SDL.h
#ifndef GLEW_H
#define GLEW_H
//#include <GL/glew.h>
#include "windows.h"

// SDL - Simple DirectMedia Layer - https://www.libsdl.org/
#include "SDL.h"
//#include "SDL_image.h"
//#include "SDL_mixer.h"
//#include "SDL_ttf.h"

// // - OpenGL Mathematics - https://glm.g-truc.net/
#define GLM_FORCE_RADIANS // force glm to use radians
// // NOTE: must do before including GLM headers
// // NOTE: GLSL uses radians, so will do the same, for consistency
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void input();
void render();

SDL_Event event;
KeyboardHandler keyboardHandler;
Triangle tri({0.0f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f, 0.5f, 0.0f, 0.0f});
Triangle tri2({0.0f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f, -0.5f, 0.0f, 0.0f});
GLuint shaderProgram;
float r, g, b = 0;
bool done = false;
Shader vSh("..//..//Assets//Shaders//shader.vert");
Shader fSh("..//..//Assets//Shaders//shader.frag");

int main(int argc, char *argv[]) {
	//SDL Initialise
	SDL_Init(SDL_INIT_EVERYTHING);

	//SDL create window
	SDL_Window *win = SDL_CreateWindow("CGP3018M OpenGL Window", 100, 100, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	//set context attributes
	//sets opengl version to 4.3
	int major = 4, minor = 3;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_PROFILE_CORE); //use core profile
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); //ask for forward compatible
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	//SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// Create an OpenGL context associated with the window.
	SDL_GLContext glcontext = SDL_GL_CreateContext(win);

	//GLEW initialise
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();

	//create, allocate and compile shaders
	//compile the shader code
	//1 for vertex, 2 for fragment - there is probably a better way to do this
	vSh.getShader(1);
	fSh.getShader(2);

	//create shader program, attach shaders together in the shader program
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vSh.shaderID);
	glAttachShader(shaderProgram, fSh.shaderID);
	glLinkProgram(shaderProgram);

	//delete shader source code pointers
	glDeleteShader(vSh.shaderID);
	glDeleteShader(fSh.shaderID);

	//OpenGL buffers
	//set buffers for the triangle
	tri.setBuffers();
	tri2.setBuffers();

	//*****************************
	//'game' loop
	while (!done)
	{
		input();
		render();

		SDL_GL_SwapWindow(win);
	}
	//****************************
	// Once finished with OpenGL functions, the SDL_GLContext can be deleted.
	SDL_GL_DeleteContext(glcontext);

	SDL_Quit();
	return 0;
}

void input()
{
	while (SDL_PollEvent(&event))
	{
		keyboardHandler.handleKeyboardEvent(event); // Useful when capturing multiple keys pressed/held and then used for other single presses for consistency

		if (event.type == SDL_QUIT)
		{
			done = true;
		}
		else if (event.type == SDL_KEYDOWN)
		{
			if (keyboardHandler.isPressed(SDLK_1))
			{
				r = 1;
				g = 0;
				b = 0;
			}
			else if (keyboardHandler.isPressed(SDLK_2))
			{
				r = 0.75;
				g = 0;
				b = 1;
			}
		}
	}
}

void render()
{
	GLint loc = glGetUniformLocation(shaderProgram, "uTime");
	if (loc != -1) // If found
	{
		glUniform1f(loc, SDL_GetTicks() / 1000.0f);
	}
	
	glClearColor(r, g, b, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	//draw the triangles
	//Use shader program we have compiled and linked
	glUseProgram(shaderProgram);

	tri.render();
	tri2.render();
}

#endif