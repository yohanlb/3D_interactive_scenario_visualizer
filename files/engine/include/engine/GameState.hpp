#pragma once

/* engine */
#include <engine/ApplicationState.hpp>
#include <engine/MenuState.hpp>
#include <engine/StopState.hpp>
#include <engine/Scene.hpp>

namespace engine {

	class GameState : public ApplicationState {
		public:
            /* Constructors & Destructor */
			GameState();
			virtual ~GameState();

            /* Other methods */
            virtual const ApplicationState* enter(Game& game) const;
            virtual ApplicationState* loop(Game& game);

		private:
            /* Other methods */
            virtual ApplicationState* processInput(Game& game);
            virtual void update(Game& game) const;
            virtual void render(Game& game) const;
	};

}
