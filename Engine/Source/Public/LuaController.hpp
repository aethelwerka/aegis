#pragma once

#include "Context.hpp"
#include "Context_GLFW3.hpp"

#include <iostream>
#include <string>

#include <sol.hpp>

#include "TextRenderer.hpp"

namespace core {

	class LuaController {
	private:
		sol::state lua;
	public:
		LuaController()
		{
			lua.open_libraries(sol::lib::base, sol::lib::os, sol::lib::package);
			lua.script("package.path = './Data/Modules/?.lua;' .. package.path");
			lua.script("load = require");
			
			initialize();
		}

		void initialize()
		{
			gl::Blend::BindToLua(lua);
			glfw3::Context::BindToLua(lua);
			TextRenderer::BindToLua(lua);

			lua.new_usertype<glm::vec3>("vec3",
				sol::constructors<glm::vec3(), glm::vec3(float), glm::vec3(float, float, float)>()
			);

			lua.new_usertype<glm::vec2>("vec2",
				sol::constructors<glm::vec2(), glm::vec3(float), glm::vec2(float, float)>()
				);

			std::cout << "Lua Script Controller: Initialized" << std::endl;
		}

		void run(std::string scriptname) {
			std::cout << "Lua Script Controller: Running " << scriptname << std::endl;
			lua.script_file(scriptname);
		}

		sol::state_view getState() const {
			return this->lua;
		}
	};
}