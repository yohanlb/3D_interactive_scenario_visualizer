#pragma once

#define BLUR_VALUE 0.015

/* engine */
#include <engine/ApplicationState.hpp>
#include <engine/MenuState.hpp>
#include <engine/StopState.hpp>
#include <engine/Scene.hpp>

/* SDL, GL, glm */
#include <GL/glew.h>

namespace engine {

	class GameState : public ApplicationState {
		public:
            /* Constructors & Destructor */
			GameState();
			virtual ~GameState();

            /* Other methods */
			GLuint generateAttachmentTexture(const Game& game, const GLboolean depth, const GLboolean stencil) const; // Generate a texture that is suited for attachments to a framebuffer
            virtual const ApplicationState* enter(Game& game);
            virtual ApplicationState* loop(Game& game);

		private:
            /* Other methods */
            virtual ApplicationState* processInput(Game& game);
            virtual void update(Game& game) const;
            virtual void render(Game& game) const;

			GLuint m_quadVAO;
			GLuint m_quadVBO;
			GLuint m_framebuffer;
			GLuint m_textureColorbuffer;
			GLfloat m_quadVertices[24];
			GLfloat m_blurValue;
	};

}
