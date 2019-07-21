#pragma once

// Model Classes
#include "Context.hpp"

// C++ Standard Library
#include <map>
#include <vector>

namespace core {
	#define KEY_A                  65
	#define KEY_B                  66
	#define KEY_C                  67
	#define KEY_D                  68
	#define KEY_E                  69
	#define KEY_F                  70
	#define KEY_G                  71
	#define KEY_H                  72
	#define KEY_I                  73
	#define KEY_J                  74
	#define KEY_K                  75
	#define KEY_L                  76
	#define KEY_M                  77
	#define KEY_N                  78
	#define KEY_O                  79
	#define KEY_P                  80
	#define KEY_Q                  81
	#define KEY_R                  82
	#define KEY_S                  83
	#define KEY_T                  84
	#define KEY_U                  85
	#define KEY_V                  86
	#define KEY_W                  87
	#define KEY_X                  88
	#define KEY_Y                  89
	#define KEY_Z                  90

	class EventManager {
	private:
		Context &context;
		std::vector<int> game_keys;
		std::map<std::string, int> keys;
	public:
		EventManager(Context &context) : context(context)
		{
			for (int key = KEY_A; key < KEY_Z; key++) {
				game_keys.emplace_back(key);
			}

			keys["PAUSE_KEY"] = KEY_P;
			keys["CONFIG_KEY"] = KEY_O;
		}

		void update() {
			if (context.keyboard(keys["PAUSE_KEY"]))
			{
				std::cout << keys.find("PAUSE_KEY")->first << ", "
					<< keys.find("PAUSE_KEY")->second << " was pressed." << std::endl;
			}
		}
	private:
		void configureKey(std::string key)
		{
			std::cout << "Press key " << key << "\n";
			keys[key] = getKeyID();
		}

		/* Returns the ID of a pressed Game Key */
		int getKeyID()
		{
			for (auto key : game_keys) {
				if (context.keyboard(key)) {
					return key;
				}
			}
		}
	};
}