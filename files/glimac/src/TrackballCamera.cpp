#include "glimac/TrackballCamera.hpp"

namespace glimac {
	
	TrackballCamera::TrackballCamera():
		m_fDistance(-5.f), m_fAngleX(0.f), m_fAngleY(0.f) {}

	void TrackballCamera::moveFront(float delta) {
		m_fDistance += delta;
	}
	
	void TrackballCamera::rotateLeft(float degrees) {
		m_fAngleY += degrees;
	}
	
	void TrackballCamera::rotateUp(float degrees) {
		m_fAngleX += degrees;
	}
	
	glm::mat4 TrackballCamera::getViewMatrix() const {
		glm::mat4 viewMatrix = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, m_fDistance));
		
		viewMatrix = glm::rotate(viewMatrix, (float)(m_fAngleX / 180 * M_PI), glm::vec3(1.f, 0.f, 0.f));
		viewMatrix = glm::rotate(viewMatrix, (float)(m_fAngleY / 180 * M_PI), glm::vec3(0.f, 1.f, 0.f));
		
		return viewMatrix;
	}

}
