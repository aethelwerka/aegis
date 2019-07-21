#pragma once

// C++ Standard Library
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>

// Third Party Libraries
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace core {

	class Shader {
	private:
		std::map<std::string, int> uniforms;
	public:
		unsigned int ID;

		Shader(const char *vertexShaderSource, const char *fragmentShaderSource) {
			compileShader(vertexShaderSource, fragmentShaderSource);
		}

		Shader(std::string filepath) {
			std::string vertexCode;
			std::string fragmentCode;

			std::ifstream vShaderFile, fShaderFile;
			vShaderFile.open(filepath + ".vs");
			fShaderFile.open(filepath + ".fs");

			// ensure ifstream objects can throw exceptions:
			vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

			std::stringstream vShaderStream, fShaderStream;
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			// close file handlers
			vShaderFile.close();
			fShaderFile.close();

			// convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();

			compileShader(vertexCode.c_str(), fragmentCode.c_str());
		}

		void compileShader(const char *vertexShaderSource, const char *fragmentShaderSource) {
			// build and compile our shader program
			// ------------------------------------
			// vertex shader
			int vertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
			glCompileShader(vertexShader);
			// check for shader compile errors
			int success;
			char infoLog[512];
			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
			}
			// fragment shader
			int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
			glCompileShader(fragmentShader);
			// check for shader compile errors
			glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
			}
			// link shaders
			ID = glCreateProgram();
			glAttachShader(ID, vertexShader);
			glAttachShader(ID, fragmentShader);
			glLinkProgram(ID);
			// check for linking errors
			glGetProgramiv(ID, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(ID, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
			}

			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
		}

		void bind() {
			glUseProgram(ID);
		}

		void setMat4(int uniformLocation, glm::mat4 mat) const {
			glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(mat));
		}

		void setVec3(int uniformLocation, const glm::vec3 &value) const {
			glUniform3fv(uniformLocation, 1, &value[0]);
		}

		int getUniformLocation(const char *name) {
			if (!uniforms.count(name)) {
				uniforms.emplace(name, (int)glGetUniformLocation(ID, name));
			}
			return uniforms.at(name);
		}
	};
}
