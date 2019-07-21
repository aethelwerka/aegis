#pragma once

// Super Class
#include "Context.hpp"

// C++ Standard Library
#include <string>
#include <iostream>

// Third Party Libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <sol.hpp>

namespace gl {
	namespace Blend {
		const unsigned int Opaque = 0;
		const unsigned int Alpha = 1;

		void BindToLua(sol::state_view lua) {
			lua["Blend"] = lua.create_table();
			lua["Blend"]["Alpha"] = Blend::Alpha;
			lua["Blend"]["Opaque"] = Blend::Opaque;
		}
	}
}

namespace glfw3 {
	#define GLFW3_ERROR_EXIT 3

	class Context : public core::Context {
	private:
		GLFWwindow *window = nullptr;
		int width, height;
		std::string title;
		double scroll_y, scroll_x;
		float field_of_view = 45.0f;
	public:
		Context(const char* title, int width, int height)
			: width(width), height(height), title(title)
		{
			glfwSetErrorCallback(error_callback);

			if (!glfwInit()) {
				exit(GLFW3_ERROR_EXIT);
			}
			atexit(glfwTerminate);

			glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // 4
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 5

#ifdef __APPLE__
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
			window = glfwCreateWindow(width, height, title, NULL, NULL);
			if (!window) {
				// Window or OpenGL context creation failed
				exit(GLFW3_ERROR_EXIT);
			}

			glfwMakeContextCurrent(window);

			glfwSetWindowUserPointer(window, this);

			auto scroll_function = [](GLFWwindow* w, double x, double y)
			{
				static_cast<Context*>(glfwGetWindowUserPointer(w))->scroll_callback(w, x, y);
			};

			glfwSetScrollCallback(window, scroll_function);

			scroll_y = 0;

			// glad: load all OpenGL function pointers
			// ---------------------------------------
			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
				//std::cout << "Failed to initialize GLAD" << std::endl;
				exit(GLFW3_ERROR_EXIT);
			}
			
			glfwSwapInterval(1);

			// TODO: OpenGL error checks

			// Check OpenGL properties
			printf("OpenGL loaded\n");
			printf("Vendor:   %s\n", glGetString(GL_VENDOR));
			printf("Renderer: %s\n", glGetString(GL_RENDERER));
			printf("Version:  %s\n", glGetString(GL_VERSION));

			char *version = (char*)glGetString(GL_VERSION);
			glfwSetWindowTitle(window, (std::string(title)
				+ " | using GLFW3 and OpenGL " + std::string(version)).c_str());

			// Enable depth test and back face culling.
			glEnable(GL_DEPTH_TEST);
			//glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glViewport(0, 0, width, height);

			//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}

		~Context() 
		{
			glfwDestroyWindow(window);
		}

		virtual void clear() final override
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		
		virtual void refresh() final override
		{
			glfwSwapBuffers(window);
		}

		virtual bool running() final override
		{
			glfwPollEvents();
			return !glfwWindowShouldClose(window);
		}

		virtual int get_width() final override {
			return (int) width;
		}

		virtual int get_height() final override {
			return (int) height;
		}

		virtual bool keyboard(int key) final override {
			return glfwGetKey(window, key);
		}

		virtual void cursor_pos(double &x, double &y) final override {
			glfwGetCursorPos(window, &x, &y);
		}

		virtual bool mouse_button(int button) final override {
			return (glfwGetMouseButton(window, button) == GLFW_PRESS);
		}

		virtual double get_scroll_yoffset() final override {
			double value = scroll_y;
			return value;
		}

		virtual void reset_scroll() final override {
			scroll_y = 0;
		}

		virtual double get_scroll_y() final override{
			double value = scroll_y;
			scroll_y = 0;
			return value;
		}

		// screen projection transformation
		glm::mat4 getProjectionMatrix() {
			return glm::perspective(glm::radians(field_of_view), (float)width / (float)height, 0.1f, 100.0f);
		}

		static void error_callback(int error, const char* description) {
			fprintf(stderr, "Error: %s\n", description);
		}

		void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
			scroll_x += xoffset;
			scroll_y += yoffset;
			std::cout << scroll_y << std::endl;

		}

		void SetClearColorf(float r, float g, float b) {
			glClearColor(r, g, b, 1.0f);
		}

		void SetClearColor(glm::vec3 color) {
			glClearColor(color.r, color.g, color.b, 1.0f);
		}

		void SetBlendMode(unsigned int mode) {
			switch (mode) {
			case gl::Blend::Alpha:
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				break;
			case gl::Blend::Opaque:
			default:
				glBlendFunc(GL_SRC_COLOR, GL_ONE);
				break;
			}
		}

		static void BindToLua(sol::state_view lua) {
			// make usertype metatable
			lua.new_usertype<glfw3::Context>("Context",
				// constructor
				sol::constructors<glfw3::Context(const char*, int, int)>(),

				// typical member functions
				"Sync", &Context::refresh,
				"Clear", &Context::clear,
				"Running", &Context::running,
				"SetBlendMode", &glfw3::Context::SetBlendMode,
				"KeyDown", &glfw3::Context::keyboard,
				"SetClearColor",
				sol::overload(&glfw3::Context::SetClearColorf, &glfw3::Context::SetClearColor)
				);
		}
	};

}

/*
float horizontal_fov = 90;
float vertical_fov = 30;

#pragma region Region: Tests with Horizontal FOV
if (context.keyboard(GLFW_KEY_P)) {
vertical_fov += 1;
} else if (context.keyboard(GLFW_KEY_O)) {
vertical_fov -= 1;
}

if (context.keyboard(GLFW_KEY_L)) {
horizontal_fov += 1;
} else if (context.keyboard(GLFW_KEY_K)) {
horizontal_fov -= 1;
}

if (vertical_fov < 30) vertical_fov = 30;
else if (vertical_fov > 90) vertical_fov = 90;
if (horizontal_fov < 80) horizontal_fov = 80;
else if (horizontal_fov > 103) horizontal_fov = 103;

float aspect = context.get_width() / (float)context.get_height();
float cotfovy = glm::cos(0.5f * glm::radians(vertical_fov)) / glm::sin(0.5f * glm::radians(vertical_fov));
float cotfovx = glm::cos(0.5f * glm::radians(horizontal_fov)) / glm::sin(0.5f * glm::radians(horizontal_fov));

projection = glm::perspective(glm::radians(field_of_view), aspect, 0.1f, 100.0f);
projection[0][0] = cotfovx / aspect;
projection[1][1] = cotfovy;
#pragma endregion Info:
*/