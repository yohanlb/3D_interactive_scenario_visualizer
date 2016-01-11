#include <engine/Scene.hpp>

namespace engine {
	
	/* Constructors & Destructor */
	Scene::Scene(const GLchar* skyboxName, const GLchar* objectName,  const GLchar* objectName2, const GLchar* lampName):
		m_skybox(skyboxName),
		m_objects{Model(objectName), Model(objectName2)},
		m_lamps{Model(lampName), Model("moon/moon.obj")},
		m_pointLightPositions{glm::vec3(200.0f, 200.0f, -200.0f), glm::vec3(100.0f, 05.0f, -50.0f)} {}

	Scene::~Scene() {}

	/* Other methods */
	void Scene::render(Camera& camera, const Shader* shaders) {
		// Draw the skybox first
		m_skybox.render(camera, shaders[2]);

		shaders[0].use();

		// Transformation matrices
		glm::mat4 projection = glm::perspective(camera.m_zoom, camera.m_windowWidth / (GLfloat) camera.m_windowHeight, camera.m_nearPlane, camera.m_farPlane), view = camera.getViewMatrix(), model;
		GLfloat angleHorizontal = glm::orientedAngle(glm::vec2(camera.m_frontMove.x, camera.m_frontMove.z), glm::vec2(0.0f, 1.0f)), angleHorizontalAbsDivPi = fabs(angleHorizontal) / M_PI;
		GLfloat angleVertical = glm::orientedAngle(glm::normalize(glm::vec2(camera.m_frontLook.y, 1.0f)), glm::normalize(glm::vec2(0.0f, 1.0f)));
		if(camera.m_in3DVehicle) model = glm::translate(model, glm::vec3(camera.m_position.x, camera.m_position.y, camera.m_position.z));
		model = glm::rotate(model, angleVertical - camera.m_pitchPin, camera.m_right);
		model = glm::rotate(model, angleHorizontal + camera.m_yawPin, camera.m_worldUp);
		if(camera.m_in3DVehicle) model = glm::translate(model, glm::vec3(0.0f, -5.0f, 14.0f));
		model = glm::rotate(model, (GLfloat) (angleHorizontalAbsDivPi * (-camera.m_pitchPin) + (1 - angleHorizontalAbsDivPi) * camera.m_pitchPin), camera.m_right);
		model = glm::rotate(model, -camera.m_yawPin, camera.m_worldUp);
		model = glm::scale(model, glm::vec3(0.025f, 0.025f, 0.025f));

		// Set the transformation uniforms
		glUniformMatrix4fv(glGetUniformLocation(shaders[0].m_program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shaders[0].m_program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shaders[0].m_program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		// Set the lighting uniforms
		for(GLuint i = POINT_LIGHTS_NUM; i--;) {
			glUniform3f(glGetUniformLocation(shaders[0].m_program, (static_cast<std::ostringstream&> (std::ostringstream().seekp(0) << "pointLights[" << i << "].position").str()).c_str()), m_pointLightPositions[i].x, m_pointLightPositions[i].y, m_pointLightPositions[i].z);
			glUniform3f(glGetUniformLocation(shaders[0].m_program, (static_cast<std::ostringstream&> (std::ostringstream().seekp(0) << "pointLights[" << i << "].diffuse").str()).c_str()), 1.0f, 1.0f, 1.0f);
			glUniform3f(glGetUniformLocation(shaders[0].m_program, (static_cast<std::ostringstream&> (std::ostringstream().seekp(0) << "pointLights[" << i << "].specular").str()).c_str()), 100.0f, 75.0f, 50.0f);
			glUniform1f(glGetUniformLocation(shaders[0].m_program, (static_cast<std::ostringstream&> (std::ostringstream().seekp(0) << "pointLights[" << i << "].constant").str()).c_str()), 1.0f);
			glUniform1f(glGetUniformLocation(shaders[0].m_program, (static_cast<std::ostringstream&> (std::ostringstream().seekp(0) << "pointLights[" << i << "].linear").str()).c_str()), 0.109);
			glUniform1f(glGetUniformLocation(shaders[0].m_program, (static_cast<std::ostringstream&> (std::ostringstream().seekp(0) << "pointLights[" << i << "].quadratic").str()).c_str()), 0.0128);
		}
		glUniform3f(glGetUniformLocation(shaders[0].m_program, (static_cast<std::ostringstream&> (std::ostringstream().seekp(0) << "pointLights[" << 0 << "].ambient").str()).c_str()), 500.0f, 375.0f, 250.0f);
		glUniform3f(glGetUniformLocation(shaders[0].m_program, (static_cast<std::ostringstream&> (std::ostringstream().seekp(0) << "pointLights[" << 1 << "].ambient").str()).c_str()), 400.0f, 300.0f, 250.0f);

		// Render the loaded models
		//for(GLuint i = OBJECTS_NUM; i--;) m_objects[i].render(shaders[0]);
		m_objects[0].render(shaders[0]);

		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::translate(glm::mat4(), glm::vec3(0.0f, -200.0f, -200.0f));
		glUniformMatrix4fv(glGetUniformLocation(shaders[0].m_program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		m_objects[0].render(shaders[0]);

		//std::cout << SDL_GetTicks() << std::endl;

		for (int i = 0; i < 20; ++i)
		{
			for (int j = 0; j < 20; ++j)
			{
				model = glm::translate(glm::mat4(), glm::vec3(0.0f+5*i, -10.0f, -150.0f + 5*j));
				glUniformMatrix4fv(glGetUniformLocation(shaders[0].m_program, "model"), 1, GL_FALSE, glm::value_ptr(model));
				m_objects[1].render(shaders[0]);
			}
		}

		shaders[1].use();
		// Set the transformation uniforms
		glUniformMatrix4fv(glGetUniformLocation(shaders[1].m_program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shaders[1].m_program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f,  -5.0f));
		for(GLuint i = LAMPS_NUM; i--;) {
			
			if (i==0)
			{
				model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			}
			else{
				model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
				m_pointLightPositions[1].x =  200* cos( (float)SDL_GetTicks()/2000 );
				m_pointLightPositions[1].y =  50.0f;
				m_pointLightPositions[1].z =  200* sin( (float)SDL_GetTicks()/2000 ) - 200 ;
			}
			model = glm::translate(glm::mat4(), m_pointLightPositions[i]);			
			glUniformMatrix4fv(glGetUniformLocation(shaders[1].m_program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			m_lamps[i].render(shaders[1]);
		}
	}

}
