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
    glimac::SDLWindowManager windowManager(screenWidth, screenHeight, "3D SIV");

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
    Shader objectsShader("shaders/multiple_lights.vs", "shaders/multiple_lights.fs");
    Shader lampObjectsShader("shaders/lamp.vs", "shaders/lamp.fs");

    // Load models
    Model ourModel("../../assets/models/stormtrooper/Stormtrooper.obj");
    Model lampModel("../../assets/models/moon/moon.obj");
    //Model ourModel("../../assets/models/nanosuit/nanosuit.obj");
    //Model ourModel("../../assets/models/bla.obj");



    //Mouse position
    glm::vec2 mousePos(screenWidth/2, screenHeight/2);
    glm::vec2 mouseOldPos(mousePos);
    SDL_WarpMouse(mousePos.x, mousePos.y);
    glm::vec2 mouseOffset(0.0, 0.0);
    float mouseEpsilon = 0.01;
    bool firstMouse = true;


    // Point light positions
    glm::vec3 pointLightPositions[] = {
        glm::vec3(5.0f, 0.0f, 10.0f),
        glm::vec3(0.0f, 40.0f, 0.0f)
    };


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
        glClearColor(0.00f, 0.00f, 0.00f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        objectsShader.Use(); 
        // Transformation matrices
        glm::mat4 projection = glm::perspective(camera.Zoom, (float)screenWidth/(float)screenHeight, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(objectsShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(objectsShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));


        // Set the lighting uniforms
        glUniform3f(glGetUniformLocation(objectsShader.Program, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z);
        // Point light 1
        glUniform3f(glGetUniformLocation(objectsShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);     
        glUniform3f(glGetUniformLocation(objectsShader.Program, "pointLights[0].ambient"), 0.09f, 0.09f, 0.09f);       
        glUniform3f(glGetUniformLocation(objectsShader.Program, "pointLights[0].diffuse"), 1.0f, 1.0f, 1.0f); 
        glUniform3f(glGetUniformLocation(objectsShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(objectsShader.Program, "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(objectsShader.Program, "pointLights[0].linear"), 0.109);
        glUniform1f(glGetUniformLocation(objectsShader.Program, "pointLights[0].quadratic"), 0.0032);      
        // Point light 2
        glUniform3f(glGetUniformLocation(objectsShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);     
        glUniform3f(glGetUniformLocation(objectsShader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);       
        glUniform3f(glGetUniformLocation(objectsShader.Program, "pointLights[1].diffuse"), 1.0f, 1.0f, 1.0f); 
        glUniform3f(glGetUniformLocation(objectsShader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(objectsShader.Program, "pointLights[1].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(objectsShader.Program, "pointLights[1].linear"), 0.009);
        glUniform1f(glGetUniformLocation(objectsShader.Program, "pointLights[1].quadratic"), 0.0032);      


        // Draw the loaded model
        glm::mat4 model;
        model = glm::translate(model, glm::vec3(20.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
        //model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f)); // It's a bit too big for our scene, so scale it down
        glUniformMatrix4fv(glGetUniformLocation(objectsShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        ourModel.Draw(objectsShader);       
                // Je dessine d'autre models pour faire des tests de caméra
        
                    int i;
                    for (i = 0; i < 10; ++i)
                    {         
                        model = glm::translate(model, glm::vec3(-10.0f, 0.0f,  0.0f));
                        glUniformMatrix4fv(glGetUniformLocation(objectsShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                        ourModel.Draw(objectsShader);       
                    }   
                

                    model = glm::translate(model, glm::vec3(-10.0f, 0.0f,  -5.0f));
                    for (i = 0; i < 10; ++i)
                    {         
                        model = glm::translate(model, glm::vec3(+10.0f, 0.0f,  0.0f));
                        glUniformMatrix4fv(glGetUniformLocation(objectsShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                        ourModel.Draw(objectsShader);       
                    }   

                    model = glm::translate(model, glm::vec3(+10.0f, 0.0f,  -5.0f));
                    for (i = 0; i < 10; ++i)
                    {         
                        model = glm::translate(model, glm::vec3(-10.0f, 0.0f,  0.0f));
                        glUniformMatrix4fv(glGetUniformLocation(objectsShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                        ourModel.Draw(objectsShader);       
                    }   
                

                    model = glm::translate(model, glm::vec3(-10.0f, 0.0f,  -5.0f));
                    for (i = 0; i < 10; ++i)
                    {         
                        model = glm::translate(model, glm::vec3(+10.0f, 0.0f,  0.0f));
                        glUniformMatrix4fv(glGetUniformLocation(objectsShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                        ourModel.Draw(objectsShader);       
                    }
        
                //**************************************************************************** 


        // Draw the lamps
        lampObjectsShader.Use();
        glUniformMatrix4fv(glGetUniformLocation(lampObjectsShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lampObjectsShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        for(GLuint i = 0; i < 2; i++)
        {
            model = glm::mat4();
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f)); // Downscale lamp object (a bit too large)
            glUniformMatrix4fv(glGetUniformLocation(lampObjectsShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
            lampModel.Draw(lampObjectsShader);
        }
        // Move the light
        pointLightPositions[0] += glm::vec3(0.0f, 0.0f, -0.005f);




        // Update the display
        windowManager.swapBuffers();
    }

    return EXIT_SUCCESS;
}
