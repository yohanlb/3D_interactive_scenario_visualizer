#pragma once

#define OBJECTS_NUM 1
#define LAMPS_NUM 1
#define POINT_LIGHTS_NUM 2

/* engine */
#include <engine/glm.hpp>
#include <engine/Model.hpp>
#include <engine/Camera.hpp>
#include <engine/Shader.hpp>
#include <engine/Skybox.hpp>

/* SDL, GL, glm */
#include <GL/glew.h>

/* other */
#include <string>
#include <sstream>

namespace engine {

	class Scene {
		public:
            /* Constructors & Destructor */
			Scene(const GLchar* skyboxName, const GLchar* model1Name, const GLchar* model2Name);
			~Scene();

			/* Other methods */
			void render(Camera& camera, const Shader* shaders) const;

			Skybox m_skybox;
			Model m_objects[OBJECTS_NUM];
			Model m_lamps[LAMPS_NUM];
			glm::vec3 m_pointLightPositions[POINT_LIGHTS_NUM];
	};

}
