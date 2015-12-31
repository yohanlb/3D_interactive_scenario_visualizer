#include <engine/Scene.hpp>

namespace engine {
	
	/* Constructors & Destructor */
	Scene::Scene(const GLchar* skyboxName, const GLchar* objectName, const GLchar* lampName):
		m_skybox(skyboxName),
		m_objects{Model(objectName)},
		m_lamps{Model(lampName)},
		m_pointLightPositions{glm::vec3(5.0f, 0.0f, 10.0f), glm::vec3(0.0f, 40.0f, 0.0f)} {}

	Scene::~Scene() {}

	/* Other methods */
	void Scene::render(Camera& camera, const Shader* shaders) const {
		// Draw the skybox first
		m_skybox.render(camera, shaders[2]);

		shaders[0].use();

		// Transformation matrices
		glm::mat4 projection = glm::perspective(camera.m_zoom, camera.m_windowWidth / (float) camera.m_windowHeight, camera.m_nearPlane, camera.m_farPlane);
		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -3.0f, -3.0f));

		// Set the transformation uniforms
		glUniformMatrix4fv(glGetUniformLocation(shaders[0].m_program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shaders[0].m_program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shaders[0].m_program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		// Set the lighting uniforms
		for(GLuint i = POINT_LIGHTS_NUM; i--;) {
			glUniform3f(glGetUniformLocation(shaders[0].m_program, (static_cast<std::ostringstream&> (std::ostringstream().seekp(0) << "pointLights[" << i << "].position").str()).c_str()), m_pointLightPositions[i].x, m_pointLightPositions[i].y, m_pointLightPositions[i].z);
			glUniform3f(glGetUniformLocation(shaders[0].m_program, (static_cast<std::ostringstream&> (std::ostringstream().seekp(0) << "pointLights[" << i << "].diffuse").str()).c_str()), 1.0f, 1.0f, 1.0f);
			glUniform3f(glGetUniformLocation(shaders[0].m_program, (static_cast<std::ostringstream&> (std::ostringstream().seekp(0) << "pointLights[" << i << "].specular").str()).c_str()), 1.0f, 1.0f, 1.0f);
			glUniform1f(glGetUniformLocation(shaders[0].m_program, (static_cast<std::ostringstream&> (std::ostringstream().seekp(0) << "pointLights[" << i << "].constant").str()).c_str()), 1.0f);
			glUniform1f(glGetUniformLocation(shaders[0].m_program, (static_cast<std::ostringstream&> (std::ostringstream().seekp(0) << "pointLights[" << i << "].linear").str()).c_str()), 0.109);
			glUniform1f(glGetUniformLocation(shaders[0].m_program, (static_cast<std::ostringstream&> (std::ostringstream().seekp(0) << "pointLights[" << i << "].quadratic").str()).c_str()), 0.0032);
		}
		glUniform3f(glGetUniformLocation(shaders[0].m_program, (static_cast<std::ostringstream&> (std::ostringstream().seekp(0) << "pointLights[" << 0 << "].ambient").str()).c_str()), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(shaders[0].m_program, (static_cast<std::ostringstream&> (std::ostringstream().seekp(0) << "pointLights[" << 1 << "].ambient").str()).c_str()), 0.09f, 0.09f, 0.09f);

		// Render the loaded models
		for(GLuint i = OBJECTS_NUM; i--;) m_objects[i].render(shaders[0]);

		shaders[1].use();

		// Set the transformation uniforms
		glUniformMatrix4fv(glGetUniformLocation(shaders[1].m_program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shaders[1].m_program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f,  -5.0f));
		for(GLuint i = LAMPS_NUM; i--;) {
			model = glm::translate(glm::mat4(), m_pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f)); // Downscale lamp object (a bit too large)
			glUniformMatrix4fv(glGetUniformLocation(shaders[1].m_program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			m_lamps[i].render(shaders[1]);
		}
	}

}
