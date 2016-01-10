#include "includes.hpp"

int main() {
    GLuint windowWidth = 1200, windowHeight = 675;

    // Initialize SDL
    if(0 != SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    // Enable multisampling
    if(SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1) == -1) std::cerr << SDL_GetError() << std::endl;
    else if(SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 6) == -1) std::cerr << SDL_GetError() << std::endl;

	// Open a window
    if(!SDL_SetVideoMode(windowWidth, windowHeight, 32, SDL_OPENGL)) {
        std::cerr << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }
    SDL_WM_SetCaption("3D Interactive Scenario Visualizer", nullptr);

    // Initialize glew for OpenGL3+ support
    glewExperimental = GL_TRUE;
    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return EXIT_FAILURE;
    }
    glViewport(0, 0, windowWidth, windowHeight);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    // Here lives the program
	engine::Application application(windowWidth, windowHeight);	
	application.run();

    SDL_Quit();
    return EXIT_SUCCESS;
}
