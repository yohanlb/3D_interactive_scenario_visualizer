#include "glimac/FreeflyCamera.hpp"

namespace glimac {
	
	FreeflyCamera::FreeflyCamera():
		m_Position(0.f, 0.f, 0.f),
		m_fPhi(M_PI),
		m_fTheta(0.f),
		m_FrontVector(glm::vec3()),
		m_LeftVector(glm::vec3()),
		m_UpVector(glm::vec3()) {
		computeDirectionVectors();
	}

	void FreeflyCamera::moveLeft(float t) {
		m_Position += m_LeftVector * t;
	}

	void FreeflyCamera::moveFront(float t) {
		m_Position += m_FrontVector * t;
	}
	
	void FreeflyCamera::rotateLeft(float degrees) {
		m_fPhi += degrees / 180 * M_PI;
	}
	
	void FreeflyCamera::rotateUp(float degrees) {
		m_fTheta += degrees / 180 * M_PI;
	}
	
	glm::mat4 FreeflyCamera::getViewMatrix() const {
		return glm::lookAt(m_Position, m_Position + m_FrontVector, m_UpVector);
	}
	
	void FreeflyCamera::computeDirectionVectors() {
		m_FrontVector = glm::vec3(cos(m_fTheta)*sin(m_fPhi), sin(m_fTheta), cos(m_fTheta)*cos(m_fPhi));
		m_LeftVector = glm::vec3(sin(m_fPhi + M_PI/2), 0.f, cos(m_fPhi + M_PI/2));
		m_UpVector = glm::cross(m_FrontVector, m_LeftVector);
	}

}
