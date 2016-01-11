#pragma once

#define OBJECTS_NUM 2
#define LAMPS_NUM 2
#define POINT_LIGHTS_NUM 2

/* engine */
#include <engine/glm.hpp>
#include <engine/Model.hpp>
#include <engine/Camera.hpp>
#include <engine/Shader.hpp>
#include <engine/Skybox.hpp>

/* SDL, GL, glm */
#include <GL/glew.h>
#include <glm/gtx/vector_angle.hpp>

/* other */
#include <cmath>
#include <string>
#include <sstream>

namespace engine {

	class Scene {
		public:
            /* Constructors & Destructor */
			Scene(const GLchar* skyboxName, const GLchar* objectName,  const GLchar* objectName2, const GLchar* lampName);
			~Scene();

			/* Other methods */
			void render(Camera& camera, const Shader* shaders);

			Skybox m_skybox;
			Model m_objects[OBJECTS_NUM];
			Model m_lamps[LAMPS_NUM];
			glm::vec3 m_pointLightPositions[POINT_LIGHTS_NUM];
	};

}
