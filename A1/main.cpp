//----------------------------------------------------------------//
// I used the code for assignemt1 and modified it for assigment 3 //
//----------------------------------------------------------------//

//Alejandro Garcia CPSC 591/691 A#1 F2020 Boilerplate

//Code heavily influenced by https://learnopengl.com/ tutorials

#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"

#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// window settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Light
glm::vec3 lightPosition = glm::vec3(2.f, 5.f, 8.f);

// Object's colour
glm::vec3 objectColour = glm::vec3(0.722, 0.45, 0.2);

// camera settings
Camera camera(glm::vec3(0.0f, 3.0f, 8.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Object rotation settings
glm::mat4 rotation;
float rotSpeed = 2.5f;

// User interface
int r = 1;
float z_min = 0.1;
bool useLinear = true;

float lastChange = 0.0f;
bool changeModel = false;

int modelIndex = 0;
int textureIndex = 0;
int shaderIndex = 0;
std::vector <string> models = { "../models/head.obj", "../models/bunny.obj", "../models/boss.obj", "../models/terrain-1.obj", "../models/terrain-2.obj" , "../models/terrain-3.obj" , "../models/sphere.obj" };
std::vector <string> textures = { "material1.png", "material2.png", "aerial1.png", "aerial2.png" };
std::vector <Shader> shaders;

enum MODELS {
    HEAD,
    BUNNY,
    BOSS,
    TERRAIN1,
    TERRAIN2,
    TERRAIN3,
    SPHERE,
};
enum TEXTURES {
    TEXTURE1,
    TEXTURE2,
    AERIAL1,
    AERIAL2
};
enum SHADERS {
    NEARSIL,
    PLASTIC,
    DEPTH
};
bool delayTime() {
    float delay = 0.6;
    if (glfwGetTime() - lastChange > delay) {
        lastChange = glfwGetTime();
        return true;
    }
    return false;
}


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CPSC 591/691 A1", NULL, NULL);
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

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader nearsilShader("../shaders/near-sil.vs", "../shaders/near-sil.fs");
    Shader plasticShader("../shaders/plastic-metal.vs", "../shaders/plastic-metal.fs");
    Shader depthShader("../shaders/depth-based.vs", "../shaders/depth-based.fs");
    
    // storing the shaders in a vector
    shaders.push_back(nearsilShader);
    shaders.push_back(plasticShader);
    shaders.push_back(depthShader);


    // load model(s), default model is vase.obj, can load multiple at a time
    // -----------
    while (!glfwWindowShouldClose(window)) {
        Model ourModel(models[modelIndex]);

        // Print model's name
        switch (modelIndex)
        {
        case 0:
            cout << "Model: Head" << endl;
            break;
        case 1:
            cout << "Model: Bunny" << endl;
            break;
        case 2:
            cout << "Model: Boss" << endl;
            break;
        case 3:
            cout << "Model: Terrain 1" << endl;
            break;
        case 4:
            cout << "Model: Terrain 2" << endl;
            break;
        case 5:
            cout << "Model: Terrain 3" << endl;
            break;
        }

        // Print texture
        switch (textureIndex)
        {
        case 0:
            cout << "Texture: Skin" << endl;
            break;
        case 1:
            cout << "Texture: Gold" << endl;
            break;
        case 2:
            cout << "Texture: Aerial1" << endl;
            break;
        case 3:
            cout << "Texture: Aerial2" << endl;
            break;
        }

        // Print shader
        switch (shaderIndex)
        {
        case 0:
            cout << "Shader: Near-silhouette shader" << endl;
            break;
        case 1:
            cout << "Shader: Plastic and metal shader (Phong like)" << endl;
            break;
        case 2:
            cout << "Shader : Depth-based shader" << endl;
            break;
        }
        // assign false to change model. waiting for the user to ask for a change in model texure or shader
        changeModel = false;
        // restoring rotation matrix
        rotation = glm::mat4(1.0f);
        

        // load texture from the file
        string location = "../textures-256-256/";
        unsigned int textureID = TextureFromFile(textures[textureIndex], location);

        //enable this to draw in wireframe
        // -----------
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        // Infinite render loop
        // -----------
        while (!glfwWindowShouldClose(window) && !changeModel)
        {
            // per-frame time logic
            // --------------------
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            // input
            // -----
            processInput(window);

            // Clear screen
            // ------
            glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // don't forget to enable shader before setting uniforms
            shaders[shaderIndex].use();

            //LIGHTS
            glUniform3fv(glGetUniformLocation(shaders[shaderIndex].ID, "lightPosition"), 1, glm::value_ptr(lightPosition));

            //User Interface
            shaders[shaderIndex].setBool("useLinear", useLinear);
            shaders[shaderIndex].setInt("r", r);
            shaders[shaderIndex].setFloat("near", z_min);

            //CAMERA
            glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            glm::mat4 view = camera.GetViewMatrix();
            glm::vec3 viewPos = camera.Position;
            shaders[shaderIndex].setMat4("projection", projection);
            shaders[shaderIndex].setMat4("view", view);
            shaders[shaderIndex].setVec3("viewPos", viewPos);


            //ACTION
            glm::mat4 model = rotation;// The model transformation of the mesh (controlled through arrows)
            if (modelIndex == HEAD || modelIndex == TERRAIN2 || modelIndex == TERRAIN3)
                model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));    // The default vase is a bit too big for our scene, so scale it down
            else if (modelIndex == BUNNY)
                model = glm::scale(model, glm::vec3(20.f, 20.f, 20.f));    // The default vase is a bit too big for our scene, so scale it down
            else if (modelIndex == TERRAIN1) {
                model = glm::translate(model, glm::vec3(-4.*90, -4*2.0, 4.*10.));
                model = glm::scale(model, glm::vec3(.004f, .004f, .004f));
            }
            else
                model = glm::scale(model, glm::vec3(1.f, 1.f, 1.f));    // The default vase is a bit too big for our scene, so scale it down

            
            shaders[shaderIndex].setMat4("model", model);
            shaders[shaderIndex].setVec3("objectColour", objectColour);

            ourModel.Draw(shaders[shaderIndex]);


            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            // -------------------------------------------------------------------------------
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void ProcessKeyboard(Camera_Movement dir, float deltaTime)
{
    float rotVelocity = rotSpeed * deltaTime;
    if (dir == FORWARD)
    {
        rotation = glm::rotate(rotation, -rotVelocity, glm::vec3(1.0, 0.0, 0.0));
    }
    if (dir == BACKWARD)
    {
        rotation = glm::rotate(rotation, rotVelocity, glm::vec3(1.0, 0.0, 0.0));
    }
    if (dir == LEFT)
    {
        rotation = glm::rotate(rotation, -rotVelocity, glm::vec3(0.0, 1.0, 0.0));
    }
    if (dir == RIGHT)
    {
        rotation = glm::rotate(rotation, rotVelocity, glm::vec3(0.0, 1.0, 0.0));
    }
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Modifing parameters
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) { // Press T to modify the vaule of r
        cout << "Enter r (int): " << endl;
        cin >> r;
        if (r < 0) {
            r = 0;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) { // Press Y to modify the value of z_min (only makes difference when using the depth-based shader)
        cout << "Enter Z min (float): " << endl;
        cin >> z_min;
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && !useLinear) { // Press G to linearize depth (only makes difference when using the depth-based shader)
        useLinear = true;
        cout << "Linearize depth is activated" << endl;
    }
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) { // Press H to deactevate linearizing the depth (only makes difference when using the depth-based shader)
        useLinear = false;
        cout << "Linearize depth is deactivated" << endl;
    }

    //Change model
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && modelIndex != HEAD) { // Press 1 to change the mode to head
        changeModel = true;
        modelIndex = HEAD;;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && modelIndex != BUNNY) { // Press 2 to change the mode to bunny
        changeModel = true;
        modelIndex = BUNNY;
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && modelIndex != BOSS) { // Press 3 to change the mode to boss
        changeModel = true;
        modelIndex = BOSS;
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && modelIndex != TERRAIN1) { // Press 4 to change the mode to terrain#1
        changeModel = true;
        modelIndex = TERRAIN1;
    }
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS && modelIndex != TERRAIN2) { // Press 5 to change the mode to terrain#2
        changeModel = true;
        modelIndex = TERRAIN2;
    }
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS && modelIndex != TERRAIN3) { // Press 6 to change the mode to terrain#3
        changeModel = true;
        modelIndex = TERRAIN3;
    }
    
    // Change texture
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS && textureIndex != TEXTURE1) { // Press U to change the texture to skin
        changeModel = true;
        textureIndex = TEXTURE1;
    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS && textureIndex != TEXTURE2) { // Press I to change the texture to gold
        changeModel = true;
        textureIndex = TEXTURE2;
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS && textureIndex != AERIAL1) { // Press J to change the texture to aerial#1
        changeModel = true;
        textureIndex = AERIAL1;
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS && textureIndex != AERIAL2) { // Press K to change the texture to aerial#2
        changeModel = true;
        textureIndex = AERIAL2;
    }


    // Change shader
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS && shaderIndex != NEARSIL) { // Press O to change the shader to near-silhouette
        changeModel = true;
        shaderIndex = NEARSIL;
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && shaderIndex != PLASTIC) { // Press P to change the shader to plastic and metal (phong like)
        changeModel = true;
        shaderIndex = PLASTIC;
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS && shaderIndex != DEPTH) { // Press L to change the shader to depth-based
        changeModel = true;
        shaderIndex = DEPTH;
    }

    //Camera controls
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);

    // Light controls
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera.ProcessKeyboard(CONTROL, delayTime());
        

    //Model controls
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
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
