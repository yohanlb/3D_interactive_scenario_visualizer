#pragma once

/* engine */
#include <engine/ApplicationState.hpp>
#include <engine/GameState.hpp>
#include <engine/MenuState.hpp>
#include <engine/Game.hpp>

namespace engine {

	class Application {
		public:
            /* Constructors & Destructor */
			Application(const GLuint windowWidth, const GLuint windowHeight);
			Application(const Application& application);
			~Application();
			
			/* Operators */
			Application& operator=(const Application& application);
			
            /* Other methods */
            void run();

		private:
			ApplicationState* m_state;
			Game m_game;
	};

}
