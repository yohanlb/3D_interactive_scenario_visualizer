#include <engine/Overlay.hpp>

namespace engine {
	
	/* Constructors & Destructor */
	Overlay::Overlay(const std::string faceName):
		m_ft(),
		m_face(),
		m_characters(),
		m_projection(glm::ortho(0.0f, 1066.0f, 0.0f, 600.0f)),
		m_VAO(),
		m_VBO(),
		m_shader("font.vs", "font.fs") {
		if(FT_Init_FreeType(&m_ft)) std::cout << "Error: FreeType: Could not init FreeType Library" << std::endl;
		if(FT_New_Face(m_ft, (static_cast<std::ostringstream&> (std::ostringstream().seekp(0) << "../../assets/fonts/" << faceName << ".ttf").str()).c_str(), 0, &m_face)) std::cout << "Error: FreeType: Failed to load font" << std::endl;
		//if (FT_New_Face(m_ft, "../../assets/fonts/Hack-v2_018-ttf/Hack-Regular.ttf", 0, &m_face)) std::cout << "Error: FreeType: Failed to load font" << std::endl;

		FT_Set_Pixel_Sizes(m_face, 0, 48);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

		for (GLubyte c = 128; c--;) {
			// Load character glyph 
			if (FT_Load_Char(m_face, c, FT_LOAD_RENDER)) {
				std::cout << "Error: FreeType: Failed to load Glyph" << std::endl;
				continue;
			}
			// Generate texture
			GLuint texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, m_face->glyph->bitmap.buffer);
			// Set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// Now store character for later use
			Character character = {texture, glm::ivec2(m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows), glm::ivec2(m_face->glyph->bitmap_left, m_face->glyph->bitmap_top), m_face->glyph->advance.x};
			m_characters.insert(std::pair<GLchar, Character>(c, character));
		}

		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	Overlay::~Overlay() {
		FT_Done_Face(m_face);
		FT_Done_FreeType(m_ft);
	}

	/* Other methods */
	void Overlay::renderText(const std::string text, GLfloat x, const GLfloat y, const GLfloat scale, const glm::vec3 color) {
		// Activate corresponding render state	
		m_shader.use();
		glUniformMatrix4fv(glGetUniformLocation(m_shader.m_program, "projection"), 1, GL_FALSE, glm::value_ptr(m_projection));
		glUniform3f(glGetUniformLocation(m_shader.m_program, "textColor"), color.x, color.y, color.z);
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(m_VAO);

		// Iterate through all characters
		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); ++c) {
			Character character = m_characters[*c];

			GLfloat xPos = x + character.m_bearing.x * scale, yPos = y - (character.m_size.y - character.m_bearing.y) * scale;
			GLfloat width = character.m_size.x * scale, height = character.m_size.y * scale;

			// Update VBO for each character
			GLfloat vertices[6][4] = {{xPos, yPos + height, 0.0, 0.0}, {xPos, yPos, 0.0, 1.0}, {xPos + width, yPos, 1.0, 1.0}, {xPos, yPos + height, 0.0, 0.0}, {xPos + width, yPos, 1.0, 1.0}, {xPos + width, yPos + height, 1.0, 0.0}};
			// Render glyph texture over quad
			glBindTexture(GL_TEXTURE_2D, character.m_textureId);
			// Update content of VBO memory
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			// Render quad
			glDrawArrays(GL_TRIANGLES, 0, 6);
			// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
			x += (character.m_advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
		}
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}
