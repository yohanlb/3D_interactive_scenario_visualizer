#pragma once

/* engine */
#include <engine/ApplicationState.hpp>

namespace engine {

	class StopState : public ApplicationState {
		public:
            /* Constructors & Destructor */
			StopState();
			virtual ~StopState();

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
