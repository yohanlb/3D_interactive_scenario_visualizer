#include <engine/Skybox.hpp>

namespace engine {
	
	/* Constructors & Destructor */
	Skybox::Skybox(const GLchar* skyboxName):
		m_skyboxVertices{-1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f},
		m_faces{static_cast<std::ostringstream&> (std::ostringstream().seekp(0) << "../../assets/textures/skybox/" << skyboxName << "_right.jpg").str(), static_cast<std::ostringstream&> (std::ostringstream().seekp(0) << "../../assets/textures/skybox/" << skyboxName << "_left.jpg").str(), static_cast<std::ostringstream&> (std::ostringstream().seekp(0) << "../../assets/textures/skybox/" << skyboxName << "_top.jpg").str(), static_cast<std::ostringstream&> (std::ostringstream().seekp(0) << "../../assets/textures/skybox/" << skyboxName << "_bottom.jpg").str(), static_cast<std::ostringstream&> (std::ostringstream().seekp(0) << "../../assets/textures/skybox/" << skyboxName << "_back.jpg").str(), static_cast<std::ostringstream&> (std::ostringstream().seekp(0) << "../../assets/textures/skybox/" << skyboxName << "_front.jpg").str()},
        m_skyboxVAO(),
        m_skyboxVBO(),
        m_cubemapTexture(loadCubemap()) {
		glGenVertexArrays(1, &m_skyboxVAO);
		glGenBuffers(1, &m_skyboxVBO);
		glBindVertexArray(m_skyboxVAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_skyboxVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_skyboxVertices), &m_skyboxVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glBindVertexArray(0);
	}

	Skybox::~Skybox() {}

	/* Other methods */
	void Skybox::render(Camera& camera, const Shader& shader) const {
		// Remember to turn depth writing off
		glDepthMask(GL_FALSE);

		shader.use();
		glm::mat4 viewSkybox = glm::mat4(glm::mat3(camera.getViewMatrix())); // Remove any translation component of the view matrix
		glm::mat4 projectionSkybox = glm::perspective(camera.m_zoom, camera.m_windowWidth / (GLfloat) camera.m_windowHeight, camera.m_nearPlane, camera.m_farPlane);
		glUniformMatrix4fv(glGetUniformLocation(shader.m_program, "view"), 1, GL_FALSE, glm::value_ptr(viewSkybox));
		glUniformMatrix4fv(glGetUniformLocation(shader.m_program, "projection"), 1, GL_FALSE, glm::value_ptr(projectionSkybox));

		// Skybox cube
		glBindVertexArray(m_skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(shader.m_program, "skybox"), 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glDepthMask(GL_TRUE);
	}

	GLuint Skybox::loadCubemap() {
		GLuint textureId;
		glGenTextures(1, &textureId);

		std::unique_ptr<glimac::Image> image;
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
		for(GLuint i = FACES_NUM; i--;) {
			image = glimac::loadImage(m_faces[i]);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, image->getWidth(), image->getHeight(), 0, GL_RGBA, GL_FLOAT, image->getPixels());
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		
		return textureId;
	}

}
