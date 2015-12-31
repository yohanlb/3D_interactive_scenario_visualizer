#include <engine/Model.hpp>

namespace engine {
	
	/* Constructors & Destructor */
	Model::Model(const GLchar* modelName) {
		loadModel(modelName);
	}

	Model::~Model() {}

	/* Other methods */
	// Render the model, and thus all its meshes
	void Model::render(const Shader& shader) const {
		for(GLuint i = m_meshes.size(); i--;) m_meshes[i].render(shader);
	}

	// Load a model with supported ASSIMP extensions from file and store the resulting meshes in the meshes vector
	void Model::loadModel(const GLchar* modelName) {
		// Read file via ASSIMP
		Assimp::Importer importer;
		std::string modelPath = static_cast<std::ostringstream&> (std::ostringstream().seekp(0) << "../../assets/models/" << modelName).str();
		const aiScene* scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs);
		// Check for errors
		if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
			return;
		}
		// Retrieve the directory path of the filepath
		m_directory = modelPath.substr(0, modelPath.find_last_of('/'));

		// Process ASSIMP's root node recursively
		processNode(scene->mRootNode, scene);
	}

    // Process a node in a recursive fashion. Process each individual mesh located at the node and repeat this process on its children nodes (if any).
    void Model::processNode(aiNode* node, const aiScene* scene) {
		// Process each mesh located at the current node
		for(GLuint i = node->mNumMeshes; i--;) {
			// The node object only contains indices to index the actual objects in the scene
			// The scene contains all the data, node is just to keep stuff organized (like relations between nodes)
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]]; 
			m_meshes.push_back(processMesh(mesh, scene));			
		}
		// After we've processed all of the meshes (if any) we then recursively process each of the children nodes
		for(GLuint i = 0; i < node->mNumChildren; i++) {
			processNode(node->mChildren[i], scene);
		}
	}

    Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
		// Data to fill
		std::vector<GLuint> indices;
		std::vector<Vertex> vertices;
		std::vector<Texture> textures;

		// Walk through each of the mesh's vertices
		for(GLuint i = mesh->mNumVertices; i--;) {
			Vertex vertex;
			glm::vec3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first
			// Positions
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.m_position = vector;
			// Normals
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.m_normal = vector;
			// Texture Coordinates
			if(mesh->mTextureCoords[0]) { // Does the mesh contain texture coordinates?
				glm::vec2 vec;
				// A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0)
				vec.x = mesh->mTextureCoords[0][i].x; 
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.m_texCoords = vec;
			}
			else vertex.m_texCoords = glm::vec2(0.0f, 0.0f);
			vertices.push_back(vertex);
		}
		// Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for(GLuint i = mesh->mNumFaces; i--;) {
			aiFace face = mesh->mFaces[i];
			// Retrieve all indices of the face and store them in the indices vector
			for(GLuint j = face.mNumIndices; j--;)
				indices.push_back(face.mIndices[j]);
		}
		// Process materials
		if(mesh->mMaterialIndex >= 0) {
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			// We assume a convention for sampler names in the shaders. Each diffuse texture should be named
			// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER
			// Same applies to other texture as the following list summarizes:
			// Diffuse: texture_diffuseN
			// Specular: texture_specularN
			// Normal: texture_normalN

			// 1. Diffuse maps
			std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			// 2. Specular maps
			std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}
		
		// Return a mesh object created from the extracted mesh data
		return Mesh(vertices, indices, textures);
	}

    // Check all material textures of a given type and load the textures if they're not loaded yet
    // The required info is returned as a Texture struct
    std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
		std::vector<Texture> textures;
		for(GLuint i = mat->GetTextureCount(type); i--;) {
			aiString str;
			mat->GetTexture(type, i, &str);
			// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
			GLboolean skip = false;
			for(GLuint j = m_texturesLoaded.size(); j--;) {
				if(m_texturesLoaded[j].m_path == str) {
					textures.push_back(m_texturesLoaded[j]);
					skip = true; // A texture with the same filepath has already been loaded, continue to next one (optimization)
					break;
				}
			}
			if(!skip) {   // If texture hasn't been loaded already, load it
				Texture texture;
				texture.m_id = textureFromFile(str.C_Str());
				texture.m_type = typeName;
				texture.m_path = str;
				textures.push_back(texture);
				m_texturesLoaded.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures
			}
		}
		return textures;
	}

    GLint Model::textureFromFile(const char* path) const {
		//Generate texture ID and load texture data 
		// TODO use filepath class
		std::string filename = std::string(path);
		filename = m_directory + '/' + filename;
		GLuint textureId;

		glGenTextures(1, &textureId);
		std::unique_ptr<glimac::Image> image = glimac::loadImage(filename.c_str());
		// Assign texture to ID
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->getWidth(), image->getHeight(), 0, GL_RGBA, GL_FLOAT, image->getPixels());
		glGenerateMipmap(GL_TEXTURE_2D);    

		// Parameters
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		return textureId;
	}

}
