#pragma once

/* engine */
#include <engine/Shader.hpp>

/* assimp */
#include <assimp/types.h>

/* SDL, GL, glm */
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/* other */
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

struct Vertex {
	Vertex():
		m_position(),
		m_normal(),
		m_texCoords() {}

    glm::vec3 m_position;
    glm::vec3 m_normal;
    glm::vec2 m_texCoords;
};

struct Texture {
	Texture():
		m_id(),
		m_type(),
		m_path() {}

    GLuint m_id;
    std::string m_type;
    aiString m_path;
};

namespace engine {

	class Mesh {
		public:
            /* Constructors & Destructor */
			Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const std::vector<Texture>& textures);
			~Mesh();

			/* Other methods */
			void render(const Shader& shader) const;

			std::vector<Vertex> m_vertices;
			std::vector<GLuint> m_indices;
			std::vector<Texture> m_textures;

		private:
			/* Other methods */
			// Initialize all the buffer objects/arrays
			void setupMesh();

			GLuint m_VAO, m_VBO, m_EBO;
	};

}
