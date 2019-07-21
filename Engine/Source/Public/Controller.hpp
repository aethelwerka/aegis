#pragma once

// Model Classes
#include "Context.hpp"
#include "Context_SDL2.hpp"
#include "Context_GLFW3.hpp"
#include "ThirdPersonCamera.hpp"
#include "Shader.hpp"

#include "Object.hpp"

// View Classes
#include "TextRenderer.hpp"

// Controller Classes
#include "EventManager.hpp"

// C++ Standard Library
#include <tuple>
#include <iostream>

// Third Party Libraries
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace core {
	class Controller {
	private:
		Context & context;
		ThirdPersonCamera camera;

		int previous_x_position = 0;
		int previous_y_position = 0;
		float xpos = 0, ypos = 0;

		glm::vec2 borders;
		Shader basicShader, myShader;
		Object quad;
		TextRenderer text;

		EventManager eventmgr;
	public:
		Controller(Context& context)
			: context(context)
			, camera(ThirdPersonCamera(glm::vec3(0), glm::vec3(0.0f, 1.0f, 0.0f)))
			, basicShader("Data/Shaders/basic")
			, myShader("Data/Shaders/shader")
			, text(context)
			, eventmgr(context)
			, quad() {
			context.clear();
			context.refresh();

			quad.on_wireframe = true;
			
			borders = glm::vec2(12.f, 9.f);
		};

		void run() {
			this->events();

			this->update();

			myShader.bind();

			context.clear();

			this->render();

			context.refresh();
		}

		void setContext(Context &context) {
			context = context;
		}

		bool running() {
			return context.running();
		}

		void events() {
			eventmgr.update();
			double x, y;
			context.cursor_pos(x, y);

			{
				// Left Mouse Button
				if (context.mouse_button(0)) {
					float angleChange = x * 0.01f;

					if (previous_x_position - x > 0) {
						camera.angle -= angleChange;
					} else if (previous_x_position - x < 0) {
						camera.angle += angleChange;
					}

					previous_x_position = x;
				}

				// Right Mouse Button
				if (context.mouse_button(1)) {
					float angleChange = y * 0.01f;

					if (previous_y_position - y > 1) {
						camera.pitch -= angleChange;
					} else if (previous_y_position - y < -1) {
						camera.pitch += angleChange;
					}

					previous_y_position = y;
				}

				camera.distance -= context.get_scroll_y();

				if (context.keyboard(GLFW_KEY_MINUS)) {
					camera.distance -= 0.1f;
				}		
				else if (context.keyboard(GLFW_KEY_EQUAL)) {
					camera.distance += 0.1f;
				}

				if (context.keyboard(GLFW_KEY_W)) {
					ypos -= 0.1f;
				} 			
				else if (context.keyboard(GLFW_KEY_S)) {
					ypos += 0.1f;
				}

				if (context.keyboard(GLFW_KEY_A)) {
					std::cout << "A" << std::endl;
					xpos -= 0.1f;
				}
				else if (context.keyboard(GLFW_KEY_D)) {
					xpos += 0.1f;
				}

				if (xpos > borders.x - 1) {
					xpos = 11;
				}
				else if (xpos < -(borders.x - 1)) {
					xpos = -11;
				}

				if (ypos > borders.y - 1) {
					ypos = 8;
				}
				else if (ypos < -(borders.y - 1)) {
					ypos = -8;
				}

				if (context.keyboard(GLFW_KEY_R)) {
					camera.reset();
				}

				if (context.keyboard(GLFW_KEY_F)) {
					quad.on_wireframe = false;
				}

				if (context.keyboard(GLFW_KEY_G)) {
					quad.on_wireframe = true;
				}
			}
		}

		void update() {
			camera.setTarget(glm::vec3(0.5f, 0.0f, 0.5f)); // center screen
														   // camera.setTarget(glm::vec3(xpos, 0.1f, ypos)); // center player

			camera.update();
		}

		void render() {
#pragma region Region: GameRenderer
			basicShader.bind();

			basicShader.setMat4(basicShader.getUniformLocation("projection"), context.getProjectionMatrix());
			basicShader.setMat4(basicShader.getUniformLocation("view"), camera.getViewMatrix());

			glm::mat4 translation = glm::translate(glm::mat4(), glm::vec3(xpos, 0.01f, ypos));
			glm::mat4 rotate = glm::rotate(glm::mat4(), glm::radians(90.f), glm::vec3(1, 0, 0));
			glm::mat4 modelViewMatrix = translation * rotate;

			basicShader.setMat4(basicShader.getUniformLocation("model"), modelViewMatrix);

			basicShader.setVec3(basicShader.getUniformLocation("color"), glm::vec3(0, 0, 1));
			quad.render();
#pragma endregion Info: All game drawn calls are located here  
			/*
#pragma region Region: UIRenderer
			glm::vec2 text_pos, text_size;

			std::string engine_name = u8"Athena Game Engine";// Çç 이렇게 三 ∃y ∀x ¬(x ≺ y)";
			std::string engine_version = u8"SandBox, Alpha 0.0.01";

			text_size = text.getTextSizePixels(engine_name, 0.3f);
			text_pos.x = 1280 / 2 - text_size.x / 2;
			text_pos.y = 720 - 25 - text_size.y;
			text.RenderText(engine_name, text_pos, 0.3f, glm::vec3(1, 0, 1));

			text_size = text.getTextSizePixels(engine_version, 0.5f);
			text_pos.x = 1280 - 25 - text_size.x;
			text_pos.y = 25.0f;
			text.RenderText(engine_version, text_pos, 0.5f, glm::vec3(0.3, 0.3, 0.3));

			text.RenderChar('ç', text_pos, 1, glm::vec3(0.3, 0.3, 0.3));

	#pragma endregion Info: All UI drawn calls are located here 
	*/
		}
	};


}