#pragma once

/* SDL, GL, glm */
#include <GL/glew.h>

/* other */
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

namespace engine {

	class Shader {
		public:
            /* Constructors & Destructor */
			Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
			~Shader();

			/* Other methods */
			// Use the shader's program
			void use() const;

			GLuint m_program;
	};

}
