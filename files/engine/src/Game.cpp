#include <engine/Game.hpp>

namespace engine {
	
	/* Constructors & Destructor */
	Game::Game(const GLuint windowWidth, const GLuint windowHeight):
		m_scene("lake", "stormtrooper/Stormtrooper.obj", "moon/moon.obj"),
		m_shaders{Shader("multiple_lights.vs", "multiple_lights.fs"), Shader("lamp.vs", "lamp.fs"), Shader("skybox.vs", "skybox.fs")},
		m_camera(windowWidth, windowHeight),
		m_overlays{Overlay("Purisa"), Overlay("Hack-v2_018-ttf/Hack-Regular")} {}

	Game::~Game() {}

}
