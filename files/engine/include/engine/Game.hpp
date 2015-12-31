#pragma once

#define SHADERS_NUM 3
#define OVERLAYS_NUM 2

/* engine */
#include <engine/Scene.hpp>
#include <engine/Shader.hpp>
#include <engine/Camera.hpp>
#include <engine/Overlay.hpp>

namespace engine {

	class Game {
		public:
            /* Constructors & Destructor */
			Game(const GLuint windowWidth, const GLuint windowHeight);
			~Game();

			Scene m_scene;
			Shader m_shaders[SHADERS_NUM];
			Camera m_camera;
			Overlay m_overlays[OVERLAYS_NUM];
	};

}
