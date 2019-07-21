#pragma once 

#include <glm/glm.hpp>

namespace core {

	class Context {
	public:
		virtual void clear() = 0;
		virtual void refresh() = 0;
		virtual bool running() = 0;

		virtual bool keyboard(int key) = 0;
		virtual void cursor_pos(double &x, double &y) = 0;
		virtual bool mouse_button(int button) = 0;

		virtual void reset_scroll() = 0;
		virtual double get_scroll_y() = 0;
		virtual double get_scroll_yoffset() = 0;

		virtual int get_width() = 0;
		virtual int get_height() = 0;

		virtual glm::mat4 getProjectionMatrix() = 0;
	};
}