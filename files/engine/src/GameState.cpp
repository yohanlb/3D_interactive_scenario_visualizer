#include <engine/GameState.hpp>
#include <iostream>
namespace engine {
	
	/* Constructors & Destructor */
	GameState::GameState() {}

	GameState::~GameState() {}

	/* Other methods */
	const ApplicationState* GameState::enter(Game& game) const {		
		return this;
	}
	
	ApplicationState* GameState::loop(Game& game) {
		GLfloat previous = GET_TIME;
		GLfloat lag = 0.0;
		ApplicationState* state = nullptr;
		
		// Application loop
		while(!m_done) {
			GLfloat current = GET_TIME;
			GLfloat elapsed = current - previous;
			previous = current;
			lag += elapsed;
			
			state = processInput(game);
			if(state != nullptr) break;

			while(lag >= UPDATE_STEP) {
				update(game);
				lag -= UPDATE_STEP;
			}
			game.m_camera.direct();
			
			render(game);
			//render(lag / MS_PER_UPDATE);
		}
		
		return state;
	}

	ApplicationState* GameState::processInput(Game& game) {
		// Calculate the mouse movement and keep the cursor centered
		game.m_camera.processMouseMovement();
		
        // Event loop
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
			switch(e.type) {
				case SDL_QUIT:
					m_done = true; // Leave the loop after this iteration
					break;
				case SDL_KEYDOWN:
					switch(e.key.keysym.sym) {
						case 'z':
						case 's':
						case 'q':
						case 'd':
							game.m_camera.processKeyboard(e.key.keysym.sym, 1);
							break;
						case SDLK_SPACE:
							game.m_scene = *(new Scene("greenLake", "stormtrooper/Stormtrooper.obj", "moon/moon.obj"));
							break;
						case SDLK_TAB:
							return new MenuState();
						case SDLK_ESCAPE:
							return new StopState();
						default:
							break;
					}
					break;
				case SDL_KEYUP:
					switch(e.key.keysym.sym) {
						case 'z':
						case 's':
						case 'q':
						case 'd':
							game.m_camera.processKeyboard(e.key.keysym.sym, -1);
							break;
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

	void GameState::update(Game& game) const {
        // Move camera
		game.m_camera.move(MS_PER_UPDATE);
	}

	void GameState::render(Game& game) const {
        // Clear the color buffer
        glClearColor(0.00f, 0.00f, 0.00f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Render the scene
		game.m_scene.render(game.m_camera, game.m_shaders);

		// Display the overlay
        game.m_overlays[1].renderText("GameState", 10.0f, 80.0f, 1.0f, glm::vec3(1.0f, 0.5f, 1.0f));
        game.m_overlays[1].renderText("Press TAB to switch between states", 10.0f, 45.0f, 0.5f, glm::vec3(1.0f, 0.5f, 1.0f));
        game.m_overlays[1].renderText("Press ESC to quit the program", 10.0f, 15.0f, 0.5f, glm::vec3(1.0f, 0.5f, 1.0f));

		// Hide the mouse cursor
		SDL_ShowCursor(SDL_DISABLE);

        // Update the display
        SDL_GL_SwapBuffers();
	}

}
