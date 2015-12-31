#include <engine/Mesh.hpp>

namespace engine {
	
	/* Constructors & Destructor */
	Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const std::vector<Texture>& textures):
		m_vertices(vertices),
		m_indices(indices),
		m_textures(textures),
		m_VAO(),
		m_VBO(),
		m_EBO() {
		// Now that we have all the required data, set the vertex buffers and its attribute pointers
		setupMesh();
	}

	Mesh::~Mesh() {}

	/* Other methods */
	void Mesh::render(const Shader& shader) const {
        // Bind appropriate textures
        GLuint diffuseNr = 1;
        GLuint specularNr = 1;
        for(GLuint i = m_textures.size(); i--;) {
			// Active proper texture unit before binding
            glActiveTexture(GL_TEXTURE0 + i);
            // Retrieve texture number (the N in diffuse_textureN)
            std::stringstream ss;
            std::string number;
            std::string name = m_textures[i].m_type;
            // Transfer GLuint to stream
            if(name == "texture_diffuse") ss << diffuseNr++;
            else if(name == "texture_specular") ss << specularNr++;
            number = ss.str(); 
            // Now set the sampler to the correct texture unit
            // added "material because multiple_lights.fs use a "material" struct"
            glUniform1i(glGetUniformLocation(shader.m_program, ("material." + name + number).c_str()), i);
            // And finally bind the texture
            glBindTexture(GL_TEXTURE_2D, m_textures[i].m_id);
        }
        
        // Also set each mesh's shininess property to a default value (if you want you could extend this to another mesh property and possibly change this value)
        glUniform1f(glGetUniformLocation(shader.m_program, "material.shininess"), 16.0f);

        // Draw mesh
        glBindVertexArray(m_VAO);
        glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Always good practice to set everything back to defaults once configured.
        for (GLuint i = m_textures.size(); i--;) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
	}

	void Mesh::setupMesh() {
        // Create buffers / arrays
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_EBO);

        glBindVertexArray(m_VAO);
        // Load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        // A great thing about structs is that their memory layout is sequential for all its items.
        // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
        // again translates to 3/2 floats which translates to a byte array.
        glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);  

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), &m_indices[0], GL_STATIC_DRAW);

        // Set the vertex attribute pointers
        // Vertex Positions
        glEnableVertexAttribArray(0);	
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, m_position));
        // Vertex Normals
        glEnableVertexAttribArray(1);	
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, m_normal));
        // Vertex Texture Coords
        glEnableVertexAttribArray(2);	
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, m_texCoords));

        glBindVertexArray(0);
	}

}
