#include <engine/MenuState.hpp>

namespace engine {
	
	/* Constructors & Destructor */
	MenuState::MenuState() {}

	MenuState::~MenuState() {}

	/* Other methods */
	const ApplicationState* MenuState::enter(Game& game) const {
		return this;
	}
	
	ApplicationState* MenuState::loop(Game& game) {
		ApplicationState* state = nullptr;
		
		// Application loop
		while(!m_done) {
			GLfloat start = GET_TIME;
			
			state = processInput(game);
			if(state != nullptr) break;
			update(game);
			render(game);
			
			SDL_Delay((GLuint)(start + UPDATE_STEP - GET_TIME));
		}
		
		return state;
	}

	ApplicationState* MenuState::processInput(Game& game) {
        // Event loop
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
			switch(e.type) {
				case SDL_QUIT:
					m_done = true; // Leave the loop after this iteration
					break;
				case SDL_KEYDOWN:
					switch(e.key.keysym.sym) {
						case SDLK_TAB:
							return new GameState();
						case SDLK_ESCAPE:
							return new StopState();
						default:
							break;
					}
					break;
				default:
					break;
			}
        }
        
        return nullptr;
	}

	void MenuState::update(Game& game) const {}

	void MenuState::render(Game& game) const {
        // Clear the color buffer
        glClearColor(0.00f, 0.00f, 0.00f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Display the overlay
        game.m_overlays[0].renderText("MenuState", 10.0f, 80.0f, 1.0f, glm::vec3(0.5f, 1.0f, 1.0f));
        game.m_overlays[0].renderText("Press TAB to switch between states", 10.0f, 45.0f, 0.5f, glm::vec3(0.5f, 1.0f, 1.0f));
        game.m_overlays[0].renderText("Press ESC to quit the program", 10.0f, 15.0f, 0.5f, glm::vec3(0.5f, 1.0f, 1.0f));
		
		// Hide the mouse cursor
		SDL_ShowCursor(SDL_ENABLE);

        // Update the display
        SDL_GL_SwapBuffers();
	}

}
