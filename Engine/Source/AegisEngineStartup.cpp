// Copyright 2018 Italo Augusto Oliveira.

#include "CoreMinimal.hpp"

int main(int argc, char **argv) {

	std::string_view view = "Aegis is Running!";
	std::cout << view << std::endl;

	/*
	core::LuaController lua;
	lua.run("Sandbox.lua");*/

	core::Context &context = glfw3::Context("Athena", 1280, 720);
	core::Controller controller(context);

	while (controller.running()) {
		try {
			controller.run();
		} catch (const std::exception& e) {
			std::cerr << "ERROR: " << e.what() << std::endl;
		}
	}

	return EXIT_SUCCESS;
}