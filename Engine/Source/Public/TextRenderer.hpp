#pragma once 

#define UNICODE
#define _UNICODE

// Model Classes
#include "Context.hpp"
#include "Shader.hpp"

// Third Party Libraries
#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
	unsigned int TextureID;		// ID handle of the glyph texture
	glm::ivec2 Size;			// Size of glyph
	glm::ivec2 Bearing;			// Offset from baseline to left/top of glyph
	unsigned int Advance;		// Horizontal offset to advance to next glyph
};

namespace core {
	class TextRenderer {
	private:
		Shader shader;
		Context &context;
		unsigned int VAO, VBO;
		std::map<unsigned char, Character> Characters;
		glm::mat4 projection;
	public:

		TextRenderer(Context &context) :
			context(context),
			shader(Shader("Data/Shaders/text"))
		{
			projection = glm::ortho(0.0f, static_cast<float>(context.get_width()), 0.0f, static_cast<float>(context.get_height()));
			
			shader.bind();
			shader.setMat4(shader.getUniformLocation("projection"), projection);

			// FreeType
			FT_Library ft;
			// All functions return a value different than 0 whenever an error occurred
			if (FT_Init_FreeType(&ft))
				std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

			// Load font as face
			FT_Face face;
			if (FT_New_Face(ft, "Data/Fonts/NanumGothic.ttf", 0, &face))
				std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

			FT_Select_Charmap(face, ft_encoding_unicode);

			// Set size to load glyphs as
			FT_Set_Pixel_Sizes(face, 0, 48);

			// Disable byte-alignment restriction
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			FT_UInt index;
			FT_ULong character = FT_Get_First_Char(face, &index);

			/*
			int i = 1;

			while (index) {
				// if FT_Get_Next_Char write 0 to index then
				// have no more characters in font face
				insertChar(face, character);
				i++;

				character = FT_Get_Next_Char(face, character, &index);
				std::cout << "Loading character number " << index << std::endl;
			}*/
			
			// Load first 128 characters of ASCII set
			for (unsigned char c = 0; c < 128; c++) {
				insertChar(face, c);
			}

			glBindTexture(GL_TEXTURE_2D, 0);
			// Destroy FreeType once we're finished
			FT_Done_Face(face);
			FT_Done_FreeType(ft);

			// Configure VAO/VBO for texture quads
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}

		glm::vec2 getTextSizePixels(std::string text, float scale) {
			float width = 0, height = 0;
			std::string::const_iterator c;
			for (c = text.begin(); c != text.end(); c++) {
				Character ch = Characters[*c];
				width += (ch.Advance >> 6) * scale;
				if (ch.Size.y * scale > height) {
					height = ch.Size.y * scale;
				}
			}

			return glm::vec2(width, height);
		}

		float getWidth(std::string text, float scale) {
			return getTextSizePixels(text, scale).x;
		}

		float getHeight(std::string text, float scale) {
			return getTextSizePixels(text, scale).y;
		}

		void RenderText(std::string text, glm::vec2 position, float scale, glm::vec3 color) {
			// Activate corresponding render state	
			shader.bind();
			shader.setVec3(shader.getUniformLocation("textColor"), color);
			glActiveTexture(GL_TEXTURE0);
			glBindVertexArray(VAO);
			
			/*
			for (size_t i = 0; i < text.length(); ) {
				int cplen = 1;
				if ((text[i] & 0xf8) == 0xf0) cplen = 4;
				else if ((text[i] & 0xf0) == 0xe0) cplen = 3;
				else if ((text[i] & 0xe0) == 0xc0) cplen = 2;
				if ((i + cplen) > text.length()) cplen = 1;

				std::cout << text.substr(i, cplen) << std::endl;
				i += cplen;
			}*/
			
			// Iterate through all characters
			std::string::const_iterator c;
			for (c = text.begin(); c != text.end(); c++) {
				std::cout << *c << std::endl;
				Character ch = Characters[*c];

				float xpos = position.x + ch.Bearing.x * scale;
				float ypos = position.y - (ch.Size.y - ch.Bearing.y) * scale;

				float w = ch.Size.x * scale;
				float h = ch.Size.y * scale;
				// Update VBO for each character
				float vertices[6][4] = {
				{ xpos,     ypos + h,   0.0, 0.0 },
				{ xpos,     ypos,       0.0, 1.0 },
				{ xpos + w, ypos,       1.0, 1.0 },

				{ xpos,     ypos + h,   0.0, 0.0 },
				{ xpos + w, ypos,       1.0, 1.0 },
				{ xpos + w, ypos + h,   1.0, 0.0 }
				};
				// Render glyph texture over quad
				glBindTexture(GL_TEXTURE_2D, ch.TextureID);
				// Update content of VBO memory
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

				glBindBuffer(GL_ARRAY_BUFFER, 0);
				// Render quad
				glDrawArrays(GL_TRIANGLES, 0, 6);
				// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
				position.x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
			}

			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void RenderChar(unsigned long character, glm::vec2 position, float scale, glm::vec3 color) {
			// Activate corresponding render state	
			shader.bind();
			shader.setVec3(shader.getUniformLocation("textColor"), color);
			glActiveTexture(GL_TEXTURE0);
			glBindVertexArray(VAO);

			Character ch = Characters[character];

			float xpos = position.x + ch.Bearing.x * scale;
			float ypos = position.y - (ch.Size.y - ch.Bearing.y) * scale;

			float w = ch.Size.x * scale;
			float h = ch.Size.y * scale;
			// Update VBO for each character
			float vertices[6][4] = {
				{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
			};
			// Render glyph texture over quad
			glBindTexture(GL_TEXTURE_2D, ch.TextureID);
			// Update content of VBO memory
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			// Render quad
			glDrawArrays(GL_TRIANGLES, 0, 6);
			// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
			position.x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))

			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		static void BindToLua(sol::state_view lua) {
			// make usertype metatable
			lua.new_usertype<TextRenderer>("TextRenderer",
				// constructor
				sol::constructors<TextRenderer(glfw3::Context)>(),

				// typical member functions
				"RenderText", &RenderText,
				"getTextSize", &getTextSizePixels,
				"GetWidth", &getWidth,
				"GetHeight", &getHeight
				);
		}

		void insertChar(FT_Face &face, unsigned char c) {
				// Load character glyph 
				if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
					std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
					//continue;
				}
				// Generate texture
				unsigned int texture;
				glGenTextures(1, &texture);
				glBindTexture(GL_TEXTURE_2D, texture);
				glTexImage2D(
					GL_TEXTURE_2D,
					0,
					GL_RED,
					face->glyph->bitmap.width,
					face->glyph->bitmap.rows,
					0,
					GL_RED,
					GL_UNSIGNED_BYTE,
					face->glyph->bitmap.buffer
				);
				// Set texture options
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				// Now store character for later use
				Character character = {
					texture,
					glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
					glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
					face->glyph->advance.x
				};

				Characters.insert(std::pair<unsigned char, Character>(c, character));
		}
	};
}