#pragma once

/* engine */
#include <engine/glm.hpp>
#include <engine/Shader.hpp>

/* SDL, GL, glm */
#include <GL/glew.h>

/* freetype */
#include <ft2build.h>
#include FT_FREETYPE_H 

/* other */
#include <map>
#include <string>
#include <sstream>
#include <iostream>

struct Character {
    GLuint m_textureId; // Id handle of the glyph texture
    glm::ivec2 m_size; // Size of glyph
    glm::ivec2 m_bearing; // Offset from baseline to left/top of glyph
    GLuint m_advance; // Offset to advance to next glyph
};

namespace engine {

	class Overlay {
		public:
            /* Constructors & Destructor */
			Overlay(const std::string faceName, const GLuint windowWidth, const GLuint windowHeight);
			~Overlay();

			/* Other methods */
			void renderText(const std::string text, GLfloat x, const GLfloat y, const GLfloat scale, const glm::vec3 color);

		private:
			FT_Library m_ft;
			FT_Face m_face;
			std::map<GLchar, Character> m_characters;
			glm::mat4 m_projection;
			GLuint m_VAO;
			GLuint m_VBO;
			Shader m_shader;
	};

}
