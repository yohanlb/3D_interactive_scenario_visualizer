#pragma once

#define FACES_NUM 6
#define VERTICES_NUM 108

/* engine */
#include <engine/Camera.hpp>
#include <engine/Shader.hpp>

/* glimac */
#include <glimac/Image.hpp>

/* SDL, GL, glm */
#include <GL/glew.h>

/* other */
#include <string>
#include <sstream>

namespace engine {

	class Skybox {
		public:
            /* Constructors & Destructor */
			Skybox(const GLchar* skyboxName);
			~Skybox();

			/* Other methods */
			void render(Camera& camera, const Shader& shader) const;

		private:
			/* Other methods */
			GLuint loadCubemap();

			GLfloat m_skyboxVertices[VERTICES_NUM];
			std::string m_faces[FACES_NUM];
			GLuint m_skyboxVAO;
			GLuint m_skyboxVBO;
			GLuint m_cubemapTexture;
	};

}
