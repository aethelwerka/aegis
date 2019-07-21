#pragma once

#include "Context.hpp"

#include <glad\glad.h>
#include <SDL2/SDL.h>

#include <string>

namespace sdl2 {
	#define SDL2_ERROR_EXIT 2

	void error_exit(const char * message)
	{
		fprintf(stderr, "%s: %s\n", message, SDL_GetError());
		exit(SDL2_ERROR_EXIT);
	}

	class Context : public core::Context {
	private:
		SDL_Window *window = nullptr;
		SDL_GLContext context;
	public:
		Context(const char* title, int width, int height)
		{
			atexit(SDL_Quit);

			SDL_GL_LoadLibrary(NULL); // Default OpenGL is fine.

			// Request an OpenGL 4.5 context (should be core)
			SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
			// Also request a depth buffer
			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
			SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

			if (SDL_Init(SDL_INIT_VIDEO) < 0) {
				error_exit("Couldn't initialize SDL");
			}

			window = SDL_CreateWindow(
				title,
				SDL_WINDOWPOS_CENTERED,
				SDL_WINDOWPOS_CENTERED,
				width, height,
				SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
			if (!window) {
				error_exit("Couldn't create the window.");
			}

			context = SDL_GL_CreateContext(window);
			gladLoadGLLoader(SDL_GL_GetProcAddress);
			SDL_GL_SetSwapInterval(1);

			// Check OpenGL properties
			printf("OpenGL loaded\n");
			printf("Vendor:   %s\n", glGetString(GL_VENDOR));
			printf("Renderer: %s\n", glGetString(GL_RENDERER));
			printf("Version:  %s\n", glGetString(GL_VERSION));

			char *version = (char*)glGetString(GL_VERSION);
			SDL_SetWindowTitle(window, (std::string(title) +
				" | using SDL2 and OpenGL " + std::string(version)).c_str());

			glViewport(0, 0, width, height);
		}

		~Context()
		{
			SDL_GL_DeleteContext(context);
			SDL_DestroyWindow(window);
		}

		virtual void clear() final override
		{
			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);
		}
		
		virtual void refresh() final override
		{
			SDL_GL_SwapWindow(window);
		}

		virtual bool running() final override
		{
			return !SDL_QuitRequested();
		}
	};
}