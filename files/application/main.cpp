#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>

#include "engine/Camera.hpp"
#include "engine/Model.hpp"
#include "engine/Shader.hpp"



int main(int argc, char** argv) {
    
    // Properties
    GLuint screenWidth = 1200, screenHeight = 1000;


    // Initialize SDL and open a window
    glimac::SDLWindowManager windowManager(screenWidth, screenHeight, "GLImac");

    // Initialize glew for OpenGL3+ support
    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/


    // Camera
    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

    // Initialize GLEW to setup the OpenGL Function pointers
    glewExperimental = GL_TRUE;
    glewInit();
    glViewport(0, 0, screenWidth, screenHeight);
    glEnable(GL_DEPTH_TEST);


    // Setup and compile our shaders
    Shader shader("shaders/shader.vs", "shaders/shader.frag");

    // Load models
    Model ourModel("../../assets/models/stormtrooper/Stormtrooper.obj");
    //Model ourModel("../../assets/models/nanosuit/nanosuit.obj");
    //Model ourModel("../../assets/models/bla.obj");



    //Mouse position
    glm::vec2 mousePos(screenWidth/2, screenHeight/2);
    glm::vec2 mouseOldPos(mousePos);
    SDL_WarpMouse(mousePos.x, mousePos.y);
    glm::vec2 mouseOffset(0.0, 0.0);
    float mouseEpsilon = 0.01;
    bool firstMouse = true;



    // Application loop:
    bool done = false;
    while(!done) {
        // Event loop:
        SDL_Event e;
        while(windowManager.pollEvent(e)) {
            if(e.type == SDL_QUIT) {
                done = true; // Leave the loop after this iteration
            }
        }

        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

        // Deplacement de caméra (à deplacer)
        // Le deuxieme arguement est a changer (delta time)
        if(windowManager.isKeyPressed(SDLK_z))
        camera.ProcessKeyboard(FORWARD, 0.006);
        if(windowManager.isKeyPressed(SDLK_s))
        camera.ProcessKeyboard(BACKWARD, 0.006);
        if(windowManager.isKeyPressed(SDLK_q))
            camera.ProcessKeyboard(LEFT, 0.006);
        if(windowManager.isKeyPressed(SDLK_d))
            camera.ProcessKeyboard(RIGHT, 0.006);


        //Gestion de la souris
        mousePos.x = windowManager.getMousePosition().x;
        mousePos.y = windowManager.getMousePosition().y;     

        if(firstMouse)
        {
            mouseOldPos.x = mousePos.x;
            mouseOldPos.y = mousePos.y;
            firstMouse = false;
        }

        GLfloat coef = 1.5;
        GLfloat xoffset = (mousePos.x - mouseOldPos.x) * coef;
        GLfloat yoffset = (mousePos.y - mouseOldPos.y) * coef; 
        
        /*
        Probleme à regler : 
        Lorsque la souris sort de la fenetre, on ne peut plus connaitre sa position.
        On ne peux donc pas faire tourner la camera autant que l'on souhaite,
        on est bloqué par le bord de l'écran.
        */
        if (mousePos.x > screenWidth*0.9)
            xoffset += 1;
        if (mousePos.x < screenWidth - screenWidth*0.9)
            xoffset -= 1;



        mouseOldPos.x = mousePos.x;
        mouseOldPos.y = mousePos.y;

        camera.ProcessMouseMovement(xoffset, -yoffset);




        // Clear the colorbuffer
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use(); 

        // Transformation matrices
        glm::mat4 projection = glm::perspective(camera.Zoom, (float)screenWidth/(float)screenHeight, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Draw the loaded model
        glm::mat4 model;
        model = glm::translate(model, glm::vec3(20.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
        //model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f)); // It's a bit too big for our scene, so scale it down
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        
        ourModel.Draw(shader);       


                // Je dessine d'autre models pour faire des tests de caméra
                    int i;
                    for (i = 0; i < 10; ++i)
                    {         
                        model = glm::translate(model, glm::vec3(-10.0f, 0.0f,  0.0f));
                        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                        ourModel.Draw(shader);       
                    }   
                

                    model = glm::translate(model, glm::vec3(-10.0f, 0.0f,  -5.0f));
                    for (i = 0; i < 10; ++i)
                    {         
                        model = glm::translate(model, glm::vec3(+10.0f, 0.0f,  0.0f));
                        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                        ourModel.Draw(shader);       
                    }   

                    model = glm::translate(model, glm::vec3(+10.0f, 0.0f,  -5.0f));
                    for (i = 0; i < 10; ++i)
                    {         
                        model = glm::translate(model, glm::vec3(-10.0f, 0.0f,  0.0f));
                        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                        ourModel.Draw(shader);       
                    }   
                

                    model = glm::translate(model, glm::vec3(-10.0f, 0.0f,  -5.0f));
                    for (i = 0; i < 10; ++i)
                    {         
                        model = glm::translate(model, glm::vec3(+10.0f, 0.0f,  0.0f));
                        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                        ourModel.Draw(shader);       
                    }  
                //**************************************************************************** 






        // Update the display
        windowManager.swapBuffers();
    }

    return EXIT_SUCCESS;
}
