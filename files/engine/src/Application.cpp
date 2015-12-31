#include <engine/Application.hpp>

namespace engine {
	
	/* Constructors & Destructor */
	Application::Application(const GLuint windowWidth, const GLuint windowHeight):
		m_state(new MenuState()),
		m_game(windowWidth, windowHeight) {
		m_state->enter(m_game);
	}

	Application::Application(const Application& application):
		m_state(nullptr),
		m_game(Game(0, 0)) {
		exit(EXIT_FAILURE);
	}

	Application::~Application() {
		delete m_state;
	}

	/* Operators */
	Application& Application::operator=(const Application& application) {
		exit(EXIT_FAILURE);
	}

	/* Other methods */
	void Application::run() {
		ApplicationState* state = m_state;
		while(state != nullptr) {
			state = m_state->loop(m_game);
			if(state != nullptr) {
				delete m_state;
				m_state = state;
				if(m_state->enter(m_game) == nullptr) state = nullptr;
			}
		}
	}

}
