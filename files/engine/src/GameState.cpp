#include <engine/GameState.hpp>
#include <iostream>
namespace engine {
	
	/* Constructors & Destructor */
	GameState::GameState():
		m_quadVAO(),
		m_quadVBO(),
		m_framebuffer(),
		m_textureColorbuffer(),
		m_quadVertices{ // Vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates
			// Positions   // TexCoords
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			-1.0f,  1.0f,  0.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
		},
		m_blurValue(BLUR_VALUE) {}

	GameState::~GameState() {}

	/* Other methods */
	// Generate a texture that is suited for attachments to a framebuffer
	GLuint GameState::generateAttachmentTexture(const Game& game, const GLboolean depth, const GLboolean stencil) const {
		GLenum attachment_type;
		if(!depth && !stencil)
			attachment_type = GL_RGB;
		else if(depth && !stencil)
			attachment_type = GL_DEPTH_COMPONENT;
		else if(!depth && stencil)
			attachment_type = GL_STENCIL_INDEX;

		// Generate texture ID and load texture data 
		GLuint textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		if(!depth && !stencil) glTexImage2D(GL_TEXTURE_2D, 0, attachment_type, game.m_camera.m_windowWidth, game.m_camera.m_windowHeight, 0, attachment_type, GL_UNSIGNED_BYTE, NULL);
		else glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, game.m_camera.m_windowWidth, game.m_camera.m_windowHeight, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL); // Using both a stencil and depth test, needs special format arguments
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		return textureID;
	}

	const ApplicationState* GameState::enter(Game& game) {
		// Setup screen VAO
		glGenVertexArrays(1, &m_quadVAO);
		glGenBuffers(1, &m_quadVBO);
		glBindVertexArray(m_quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_quadVertices), &m_quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
		glBindVertexArray(0);

		// Framebuffers
		glGenFramebuffers(1, &m_framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
		m_textureColorbuffer = generateAttachmentTexture(game, false, false); // Create a color attachment texture
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureColorbuffer, 0);

		// Create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
		GLuint rbo;
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo); 
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, game.m_camera.m_windowWidth, game.m_camera.m_windowHeight); // Use a single renderbuffer object for both a depth and stencil buffer.
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Error: Framebuffer: Framebuffer is not complete !" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
							delete game.m_scene;
							game.m_scene = new Scene("lake", "Stormtrooper/Stormtrooper.obj", "moon/moon.obj");
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
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

        // Clear the color buffer
        glClearColor(0.00f, 0.00f, 0.00f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Render the scene
		game.m_scene->render(game.m_camera, game.m_shaders);

		// Display the overlay
        game.m_overlays[0].renderText("Espace", 10.0f, 110.0f, 1.0f, glm::vec3(1.0f, 0.5f, 1.0f));
        game.m_overlays[0].renderText("Menu: TAB", 10.0f, 75.0f, 0.5f, glm::vec3(1.0f, 0.5f, 1.0f));
        game.m_overlays[0].renderText("Quitter: ESC", 10.0f, 45.0f, 0.5f, glm::vec3(1.0f, 0.5f, 1.0f));
        game.m_overlays[0].renderText("Piloter: ZQSD + souris", 10.0f, 15.0f, 0.5f, glm::vec3(1.0f, 0.5f, 1.0f));

		// Hide the mouse cursor
		//SDL_ShowCursor(SDL_DISABLE);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // Clear all relevant buffers
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Draw screen
        game.m_shaders[3].use();
        glUniform1f(glGetUniformLocation(game.m_shaders[3].m_program, "blurValue"), m_blurValue);        
        glBindVertexArray(m_quadVAO);
        glBindTexture(GL_TEXTURE_2D, m_textureColorbuffer); // Use the color attachment texture as the texture of the quad plane
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        // Update the display
        SDL_GL_SwapBuffers();
	}

}
