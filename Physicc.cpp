#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include "utility/Texture.h"
#include "utility/shader.h"
#include "utility/skybox.h"
#include "utility/sphere.h"
#include <iostream>
#include "utility/camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

// Declare functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings for screen and lighting type
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 640;
bool blinn = false;
bool blinnKeyPressed = false;
bool spot = false;
bool spotKeyPressed = false;

// camera
Camera camera(glm::vec3(0.0f, 5.0f, 0.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

//for movement speed per frame
float deltatime = 0.0f;
float lastframe = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Test LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Initialise OpenGL context with GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    // Depth Buffer setting
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // build and compile shaders
    // -------------------------
    Shader skyboxShader("Shaders/skyboxshad.vs", "Shaders/skyboxshad.fs");
    Shader shader("Shaders/lightobjshad.vs", "Shaders/light_dir_shad.fs");
    Shader lightShader("Shaders/lightshad.vs", "Shaders/lightshad.fs");
    Shader sphereShader("Sphere_Shaders/sphereshader.vs", "Sphere_Shaders/sphereshadercolor.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    // Square plane of default length = 20.0f
    float planeVertices[] = {
        // positions            // normals         // texcoords
         10.0f, 0.0f,  10.0f,  0.0f, 1.0f, 0.0f,   1.0f,  0.0f,
        -10.0f, 0.0f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -10.0f, 0.0f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  1.0f,

         10.0f, 0.0f,  10.0f,  0.0f, 1.0f, 0.0f,   1.0f,  0.0f,
        -10.0f, 0.0f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  1.0f,
         10.0f, 0.0f, -10.0f,  0.0f, 1.0f, 0.0f,   1.0f,  1.0f
    };

    // Cube of default side length = 1.0f
    float vertices[] = {
        // positions          // texcoords 
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    // plane VAO
    unsigned int planeVAO, planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindVertexArray(0);

    // load textures
    // -------------
    Texture texture;
    unsigned int floorTexture = texture.loadTexture("Images/floor.png");
    unsigned int floorSpecular = texture.loadTexture("Images/floor_specular.png");

    // Material properties
    shader.use();
    shader.setInt("material.diffuse", 0);
    shader.setInt("material.specular", 1);
    shader.setFloat("material.shininess", 32.0f);

    // Material properties
    sphereShader.use();
    sphereShader.setVec3("material.diffuse", 0.706f, 0.855f, 0.333f);
    sphereShader.setVec3("material.ambient", 0.706f, 0.855f, 0.333f);
    sphereShader.setVec3("material.specular", 0.706f, 0.855f, 0.333f);
    sphereShader.setFloat("material.shininess", 8.0f);

    // Initialise sphere data
    Sphere sphere1(sphereShader.ID);

    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per frame time logic
        // --------------------
        float currentframe = glfwGetTime();
        deltatime = currentframe - lastframe;
        lastframe = currentframe;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw scene
        // ----------
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        shader.use();
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        shader.setVec3("viewPos", camera.Position); //viewer position

        //light and material properties
        //-----------------------------
        glm::vec3 pointLightPositions[] = {
            glm::vec3(9.9f, 0.5f,  9.9f),
            glm::vec3(-9.9f, 0.5f,  9.9f),
            glm::vec3(-9.9f, 0.5f,  -9.9f),
            glm::vec3(9.9f, 0.5f,  -9.9f)
        };

        // directional light
        shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        shader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        shader.setVec3("dirLight.diffuse", 0.2f, 0.2f, 0.2f);
        shader.setVec3("dirLight.specular", 0.0f, 0.0f, 0.0f);

        // blinn
        shader.setInt("blinn", blinn);
        // spotlight on/off
        shader.setInt("spot", spot);

        // draw floor
        // -----------
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, floorSpecular);
        glBindVertexArray(planeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        //spheres
        sphereShader.use();

        sphereShader.setMat4("view", view);
        sphereShader.setMat4("projection", projection);
        sphereShader.setVec3("viewPos", camera.Position); //viewer position
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));

        // directional light
        sphereShader.setVec3("light.direction", -0.2f, -1.0f, -0.3f);
        sphereShader.setVec3("light.ambient", 0.05f, 0.05f, 0.05f);
        sphereShader.setVec3("light.diffuse", 0.2f, 0.2f, 0.2f);
        sphereShader.setVec3("light.specular", 0.0f, 0.0f, 0.0f);
        sphereShader.setBool("light.directional", true);

        // Draw the sphere
        sphereShader.setMat4("model", model);
        glBindVertexArray(sphere1.sphereVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 1259);

        glBindVertexArray(0);

        std::cout << (blinn ? "Blinn-Phong" : "Phong") << std::endl;

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    // camera
    // ------------
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltatime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltatime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltatime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltatime);

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && !blinnKeyPressed)
    {
        blinn = !blinn;
        blinnKeyPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE)
    {
        blinnKeyPressed = false;
    }

    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && !spotKeyPressed)
    {
        spot = !spot;
        spotKeyPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE)
    {
        spotKeyPressed = false;
    }
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}