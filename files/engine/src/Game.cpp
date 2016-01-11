#include <engine/Game.hpp>

namespace engine {
	
	/* Constructors & Destructor */
	Game::Game(const GLuint windowWidth, const GLuint windowHeight):
		m_scene(new Scene("starfield", "Millennium_Falcon/Millennium_Falcon.obj", "Stormtrooper/Stormtrooper.obj", "sun/moon.obj")),
		m_shaders{Shader("multiple_lights.vs", "multiple_lights.fs"), Shader("lamp.vs", "lamp.fs"), Shader("skybox.vs", "skybox.fs"), Shader("advancedShader.vs", "advancedShader.fs")},
		m_camera(windowWidth, windowHeight),
		m_overlays{Overlay("Hack-v2_018-ttf/Hack-Regular", m_camera.m_windowWidth, m_camera.m_windowHeight)} {}

	Game::~Game() {
		delete m_scene;
	}

}
