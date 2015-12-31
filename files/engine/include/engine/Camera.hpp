#pragma once

/* engine */
#include <engine/glm.hpp>

/* SDL, GL, glm */
#include <SDL/SDL.h>
#include <GL/glew.h>

// Default camera values
const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 0.01f;
const GLfloat SENSITIVTY = 0.20f;
const GLfloat ZOOM = 45.0f; // Visually interesting or extreme values : 46.0f~47.0f ; 44.25f

namespace engine {

	// An abstract camera class that processes input and calculates the corresponding Euler angles, vectors and matrices for use in OpenGL
	class Camera {
		public:
            /* Constructors & Destructor */
			// Constructor with vectors
			Camera(const GLuint windowWidth, const GLuint windowHeight, const GLfloat nearPlane = 0.1f, const GLfloat farPlane = 100.0f, const glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f), const glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f), const GLfloat yaw = YAW, const GLfloat pitch = PITCH, const GLfloat movementSpeed = SPEED, const GLfloat mouseSensitivity = SENSITIVTY, const GLfloat zoom = ZOOM);
			// Constructor with scalar values
			Camera(const GLuint windowWidth, const GLuint windowHeight, const GLfloat nearPlane, const GLfloat farPlane, const GLfloat positionX = 0.0f, const GLfloat positionY = 0.0f, const GLfloat positionZ = 0.0f, const glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f), const GLfloat worldUpX = 0.0f, const GLfloat worldUpY = 1.0f, const GLfloat worldUpZ = 0.0f, const GLfloat yaw = YAW, const GLfloat pitch = PITCH, const GLfloat movementSpeed = SPEED, const GLfloat mouseSensitivity = SENSITIVTY, const GLfloat zoom = ZOOM);
			~Camera();
			
            /* Other methods */
			// Process input received from any keyboard-like input system. Takes a char to represent the key and an int to represent the event (1: key down, -1: key just released, else: case-by-case meaning)
			void processKeyboard(const GLbyte key, const GLint upPlusOneDownNegative);
			// Processes input received from a mouse input system.
			void processMouseMovement();
			// Update the position based on the keyboard-processed movement
			void move(const GLfloat deltaTime);
			// Update the directions based on the mouse-processed orientation
			void direct(const GLboolean constrainPitch = true);
			// Returns the view matrix calculated using Euler angles and the lookAt matrix
			glm::mat4 getViewMatrix() const;
            
			/* Camera attributes */
			glm::vec3 m_position;
			glm::vec3 m_frontLook;
			glm::vec3 m_frontMove;
			glm::vec3 m_up;
			glm::vec3 m_right;
			glm::vec3 m_worldUp;

			/* Euler angles */
			GLfloat m_yaw;
			GLfloat m_pitch;

			/* Camera options */
			GLuint m_windowWidth;
			GLuint m_windowHeight;
			GLfloat m_zoom;
			GLfloat m_nearPlane;
			GLfloat m_farPlane;
			
			/* Keyboard-triggered camera movement properties */
			GLfloat m_movementSpeed;
			GLfloat m_leftRightMove;
			GLfloat m_backwardForwardMove;

			/* Mouse-driven camera movement properties */
			GLfloat m_mouseSensitivity;
			glm::ivec2 m_mousePosition;
			glm::ivec2 m_mouseOldPosition;
			glm::vec2 m_mouseOffset;

		private:
			// Calculates the front vector from the Camera's (updated) Euler angles
			void updateCameraVectors();
	};

}
