#pragma once

/* engine */
#include <engine/Mesh.hpp>
#include <engine/Shader.hpp>

/* glimac */
#include <glimac/Image.hpp>

/* assimp */
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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

namespace engine {

	class Model {
		public:
            /* Constructors & Destructor */
			Model(const GLchar* modelName);
			~Model();

			/* Other methods */
			// Render the model, and thus all its meshes
			void render(const Shader& shader) const;

		private:
			/* Other methods */
			// Load a model with supported ASSIMP extensions from file and store the resulting meshes in the meshes array
			void loadModel(const GLchar* modelName);
			// Process a node in a recursive fashion. Process each individual mesh located at the node and repeat this process on its children nodes (if any)
			void processNode(aiNode* node, const aiScene* scene);
			Mesh processMesh(aiMesh* mesh, const aiScene* scene);
			// Check all material textures of a given type and load the textures if they're not loaded yet.
			// The required info is returned as a Texture struct
			std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
			GLint textureFromFile(const char* path) const;

			std::string m_directory;
			std::vector<Mesh> m_meshes;
			std::vector<Texture> m_texturesLoaded; // Store all the textures loaded so far, optimization to make sure textures aren't loaded more than once
	};

}
