#include <engine/Camera.hpp>
#include <iostream>
namespace engine {
	
	/* Constructors & Destructor */
	// Constructor with vectors
	Camera::Camera(const GLuint windowWidth, const GLuint windowHeight, const GLfloat nearPlane, const GLfloat farPlane, const glm::vec3 position, const glm::vec3 front, const glm::vec3 worldUp, const GLfloat yaw, const GLfloat pitch, const GLfloat movementSpeed, const GLfloat mouseSensitivity, const GLfloat zoom):
		m_position(position),
		m_frontLook(front),
		m_frontMove(m_frontLook),
		m_up(),
		m_right(),
		m_worldUp(worldUp),
		m_yaw(yaw),
		m_pitch(pitch),
		m_yawPin(0.0f),
		m_pitchPin(0.0f),
		m_windowWidth(windowWidth),
		m_windowHeight(windowHeight),
		m_zoom(zoom),
		m_nearPlane(nearPlane),
		m_farPlane(farPlane),
		m_in2DVehicle(false),
		m_in3DVehicle(true),
		m_movementSpeed(m_in3DVehicle ? movementSpeed / 20 : movementSpeed),
		m_leftRightMove(0),
		m_backwardForwardMove(0),
		m_backwardForwardDirection(),
		m_leftRightDirection(),
		m_mouseSensitivity(m_in3DVehicle ? mouseSensitivity / 3 : mouseSensitivity),
		m_mousePosition(m_windowWidth / 2, m_windowHeight / 2),
		m_mouseOldPosition(m_mousePosition),
		m_mouseOffset(0.0, 0.0) {
		updateCameraVectors();
	}

	Camera::~Camera() {}

	/* Other methods */
	// Process input received from any keyboard-like input system. Takes a char to represent the key and an int to represent the event (1: key down, -1: key just released, else: case-by-case meaning)
	void Camera::processKeyboard(const GLbyte key, const GLint upOrDown) {
		switch(key) {
			case 'z':
				m_backwardForwardMove +=  upOrDown;
				break;
			case 's':
				m_backwardForwardMove -= upOrDown;
				break;
			case 'q':
				m_leftRightMove -= upOrDown;
				break;
			case 'd':
				m_leftRightMove += upOrDown;
				break;
			default:
				break;
		}
	}

	// Processes input received from a mouse input system.
	void Camera::processMouseMovement() {
		SDL_GetMouseState(&m_mousePosition.x, &m_mousePosition.y);
		m_mouseOffset = m_mousePosition - m_mouseOldPosition;
		if(m_in3DVehicle) {
			if(m_mousePosition.x < m_windowWidth / 4) {
				m_mouseOffset.x -= 1;
				m_yawPin = -M_PI / 16;
			}
			else if(m_mousePosition.x > m_windowWidth / 4 * 3) {
				m_mouseOffset.x += 1;
				m_yawPin = M_PI / 16;
			}
			else m_yawPin = (m_mousePosition.x / (GLfloat) m_windowWidth * M_PI - M_PI / 2) / 4;
			if(m_mousePosition.y < m_windowHeight / 4) {
				m_mouseOffset.y -= 1;
				m_pitchPin = M_PI / 16;
			}
			else if(m_mousePosition.y > m_windowHeight / 4 * 3) {
				m_mouseOffset.y += 1;
				m_pitchPin = -M_PI / 16;
			}
			else m_pitchPin = (m_mousePosition.y / (GLfloat) m_windowHeight * M_PI - M_PI / 2) / (-4);
		}
		else m_mousePosition = glm::ivec2(m_windowWidth / 2, m_windowHeight / 2);
		m_mouseOldPosition = m_mousePosition;
	}
	
	// Update the position based on the keyboard-processed movement
	void Camera::move(const GLfloat deltaTime) {
		if(m_in3DVehicle) {
			m_backwardForwardDirection += m_frontLook * m_backwardForwardMove;
			m_leftRightDirection += m_right * m_leftRightMove;

			m_backwardForwardDirection -= m_backwardForwardDirection * DAMPING;
			m_leftRightDirection -= m_leftRightDirection * DAMPING;
			m_position += (m_backwardForwardDirection + m_leftRightDirection) * m_movementSpeed * deltaTime;
			GLboolean goingBackward = glm::orientedAngle(glm::vec2(m_frontLook.x, m_frontLook.z), glm::vec2(m_backwardForwardDirection.x, m_backwardForwardDirection.z));
			m_zoom = ZOOM + glm::l2Norm(goingBackward ? glm::vec3() : glm::vec3(fabs(m_backwardForwardDirection.x), fabs(m_backwardForwardDirection.y), fabs(m_backwardForwardDirection.z)) / 100.0f);
		}
		else {
			m_position += (m_frontMove * m_backwardForwardMove + m_right * m_leftRightMove) * m_movementSpeed * deltaTime;
		}
	}
	
	// Update the directions based on the mouse-processed orientation
	void Camera::direct(const GLboolean constrainPitch) {
		m_yaw += m_mouseOffset.x * m_mouseSensitivity;
		m_pitch += -1 * m_mouseOffset.y * m_mouseSensitivity;

		// Make sure that when m_pitch is out of bounds, the screen doesn't get flipped
		if(constrainPitch) {
			if(m_in3DVehicle) {
				if(m_pitch > 79.0f)	m_pitch = 79.0f;
				else if(m_pitch < -79.0f) m_pitch = -79.0f;
			}
			else {
				if(m_pitch > 89.0f)	m_pitch = 89.0f;
				else if(m_pitch < -89.0f) m_pitch = -89.0f;
			}
		}

		// Update m_front, m_right and m_up vectors using the updated Euler angles
		updateCameraVectors();
		if(!m_in3DVehicle) SDL_WarpMouse(m_mousePosition.x, m_mousePosition.y);
	}

	// Returns the view matrix calculated using Euler angles and the lookAt matrix
	glm::mat4 Camera::getViewMatrix() const {
		return glm::lookAt(m_position, m_position + m_frontLook, m_up);
	}

	// Calculates the front vector from the Camera's (updated) Euler angles
	void Camera::updateCameraVectors()
	{
		// Calculate the new m_front vector
		glm::vec3 front;
		front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		m_frontMove = glm::normalize(front);
		front.y = sin(glm::radians(m_pitch));
		m_frontLook = glm::normalize(front);
		// Also re-calculate the m_right and m_up vector
		m_right = glm::normalize(glm::cross(m_frontLook, m_worldUp)); // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement
		m_up = glm::normalize(glm::cross(m_right, m_frontLook));
	}

}
