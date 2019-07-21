#pragma once

// C++ Standard Library
#include <iostream>

// Third Party Libraries
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.hpp"

namespace core {

	class ThirdPersonCamera {
	private:
		glm::vec3 target;
		glm::vec3 position;
		glm::vec3 world_up;
	public:
		float distance;
		float pitch;
		float angle;

		ThirdPersonCamera(glm::vec3 target = camera::Direction,
			glm::vec3 up = camera::WorldUp)
			: target(target), world_up(up)
		{
			reset();
		}

		void reset() {
			angle = 0.0f;
			distance = 2;
			pitch = 89.f;
		}

		void update() {
			if (this->distance < 1)
				this->distance = 1;

			if (this->pitch > 89.0f)
				this->pitch = 89.0f;
			if (this->pitch < 20.0f)
				this->pitch = 20.0f;

			float h_distance = distance * glm::cos(glm::radians(pitch));
			float v_distance = distance * glm::sin(glm::radians(pitch));

			float theta = angle; /* target y rotation + angle*/
			position.y = target.y + v_distance;
			position.x = target.x - h_distance * glm::sin(glm::radians(theta));
			position.z = target.z + h_distance * glm::cos(glm::radians(theta));
		}

		/* Getter & Setters */
		glm::mat4 getViewMatrix() {
			return glm::lookAt(position, target, world_up);
		}
		glm::vec3 getPosition() { return position; }
		void setTarget(glm::vec3 newTarget) { target = newTarget; }
	};
}