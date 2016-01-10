#pragma once

/* engine */
#include <engine/ApplicationState.hpp>
#include <engine/GameState.hpp>
#include <engine/StopState.hpp>

namespace engine {

	class MenuState : public ApplicationState {
		public:
            /* Constructors & Destructor */
			MenuState();
			virtual ~MenuState();

            /* Other methods */
            virtual const ApplicationState* enter(Game& game);
            virtual ApplicationState* loop(Game& game);

		private:
            /* Other methods */
            virtual ApplicationState* processInput(Game& game);
            virtual void update(Game& game) const;
            virtual void render(Game& game) const;
	};

}
