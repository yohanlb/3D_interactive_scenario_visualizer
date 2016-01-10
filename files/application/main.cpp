#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "engine/Camera.hpp"
#include "engine/Model.hpp"
#include "engine/Shader.hpp"
#include "glimac/Image.hpp"
#include <string>
#include <vector>

using namespace glimac;
using namespace std;



GLuint loadCubemap(vector<string> faces)
{
    GLuint textureID;
    glGenTextures(1, &textureID);

   std::unique_ptr<Image> image;
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    for(GLuint i = 0; i < faces.size(); i++)
    {
        image = loadImage(faces[i]);
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
            GL_RGBA, image->getWidth(), image->getHeight(), 0, GL_RGBA, GL_FLOAT, image->getPixels()
        );
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return textureID;
}  


// Generates a texture that is suited for attachments to a framebuffer
GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil)
{
    GLuint screenWidth = 1200, screenHeight = 1000;

    // What enum to use?
    GLenum attachment_type;
    if(!depth && !stencil)
        attachment_type = GL_RGB;
    else if(depth && !stencil)
        attachment_type = GL_DEPTH_COMPONENT;
    else if(!depth && stencil)
        attachment_type = GL_STENCIL_INDEX;

    //Generate texture ID and load texture data 
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    if(!depth && !stencil)
        glTexImage2D(GL_TEXTURE_2D, 0, attachment_type, screenWidth, screenHeight, 0, attachment_type, GL_UNSIGNED_BYTE, NULL);
    else // Using both a stencil and depth test, needs special format arguments
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, screenWidth, screenHeight, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    return textureID;
}


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
    glDepthFunc(GL_LESS);


    // Setup and compile our shaders
    Shader objectsShader("shaders/multiple_lights.vs", "shaders/multiple_lights.fs");
    Shader advancedShader("shaders/advancedShader.vs", "shaders/advancedShader.fs");
    Shader lampObjectsShader("shaders/lamp.vs", "shaders/lamp.fs");
    Shader skyboxShader("shaders/skybox.vs", "shaders/skybox.fs");

    // Load models
    Model ourModel("../../assets/models/stormtrooper/Stormtrooper.obj");
    Model lampModel("../../assets/models/moon/moon.obj");
    //Model ourModel("../../assets/models/nanosuit/nanosuit.obj");
    //Model ourModel("../../assets/models/bla.obj");





      GLfloat quadVertices[] = {   // Vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
            // Positions   // TexCoords
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
             1.0f, -1.0f,  1.0f, 0.0f,

            -1.0f,  1.0f,  0.0f, 1.0f,
             1.0f, -1.0f,  1.0f, 0.0f,
             1.0f,  1.0f,  1.0f, 1.0f
        };  
     // Setup screen VAO
    GLuint quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
    glBindVertexArray(0);


    #pragma region "object_initialization"
    // Set the object data (buffers, vertex attributes) 
     
    GLfloat skyboxVertices[] = {
        // Positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
  
        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
  
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
   
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
  
        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
  
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    
    // Setup skybox VAO
    GLuint skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glBindVertexArray(0);
    #pragma endregion



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

     // Cubemap (Skybox)
    vector<string> faces;
    faces.push_back("../../assets/textures/skybox/right.jpg");
    faces.push_back("../../assets/textures/skybox/left.jpg");
    faces.push_back("../../assets/textures/skybox/top.jpg");
    faces.push_back("../../assets/textures/skybox/bottom.jpg");
    faces.push_back("../../assets/textures/skybox/back.jpg");
    faces.push_back("../../assets/textures/skybox/front.jpg");
    GLuint cubemapTexture = loadCubemap(faces); 








    // Framebuffers
    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);  

    // Create a color attachment texture
    GLuint textureColorbuffer = generateAttachmentTexture(false, false);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    // Create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    GLuint rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo); 
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight); // Use a single renderbuffer object for both a depth AND stencil buffer.
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // Now actually attach it
    // Now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    GLfloat blurValue = 0.0;






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




        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);



        // Clear the colorbuffer
        glClearColor(0.00f, 0.00f, 0.00f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        

         // Draw skybox first
        glDepthMask(GL_FALSE);// Remember to turn depth writing off
        skyboxShader.Use();     
        glm::mat4 view1 = glm::mat4(glm::mat3(camera.GetViewMatrix()));  // Remove any translation component of the view matrix
        glm::mat4 projection1 = glm::perspective(camera.Zoom, (float)screenWidth/(float)screenHeight, 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view1));
        glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection1));
        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(glGetUniformLocation(skyboxShader.Program, "skybox"), 0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthMask(GL_TRUE);
        


        objectsShader.Use(); 
        // Transformation matrices
        glm::mat4 projection = glm::perspective(camera.Zoom, (float)screenWidth/(float)screenHeight, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(objectsShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(objectsShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));


        // Set the lighting uniforms
        //glUniform3f(glGetUniformLocation(objectsShader.Program, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z);
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
         model = glm::translate(model, glm::vec3(-10.0f, 0.0f,  -5.0f));

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

        


        //2ND FRAMEBUFFER
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // Clear all relevant buffers
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Draw Screen
        advancedShader.Use();
        blurValue += 0.01;
        glUniform1f(glGetUniformLocation(advancedShader.Program, "blurValue"), blurValue);
        
        glBindVertexArray(quadVAO);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);   // Use the color attachment texture as the texture of the quad plane
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);



        // Update the display
        windowManager.swapBuffers();
    }

    return EXIT_SUCCESS;
}
