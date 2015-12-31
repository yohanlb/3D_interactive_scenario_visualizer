#pragma once

/* engine */
#include <engine/Game.hpp>
#include <engine/glm.hpp>

/* other */
#include <SDL/SDL.h>

#define GET_TIME 0.001f*SDL_GetTicks()
#define MS_PER_UPDATE 16.666666667
#define UPDATE_STEP MS_PER_UPDATE / 1000

namespace engine {

	class ApplicationState {
		public:
            /* Constructors & Destructor */
            ApplicationState();
            virtual ~ApplicationState();

            /* Other methods */
            virtual const ApplicationState* enter(Game& game) const = 0;
            virtual ApplicationState* loop(Game& game) = 0;

		private:
            /* Other methods */
            virtual ApplicationState* processInput(Game& game) = 0;
            virtual void update(Game& game) const = 0;
            virtual void render(Game& game) const = 0;
            
        protected:
            bool m_done;
	};

}
