#define GLEW_STATIC
#include <GL/glew.h>
//#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "shader.h"
#include "Camera.h"
#include "Sphere.h"
#include "objModel.h"
#include "skyboxTexture.h"
#include "groundSurface.h"
#include "Firework.h"
#include "texture.h"
#include "LSystem.h"
#include "bush.h"
#include "Rock.h"
#include "cylinder.h"
#include "snow.h"
#include "player.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processKeyboardInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void hitbox(glm::vec3 barrier, float dist);
void renderQuad();

float screenWidth=1600;
float screenHeight=1200;
Camera camera(glm::vec3(0.0f,5.0f,10.0f));

bool IsSuccess = false;
bool IsFree = false;
bool RestartSnow_1 = false;
bool RestartSnow_2 = false;

float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间

float lastX = screenWidth/2;
float lastY = screenHeight/2;
bool firstMouse = true;

//glm::vec3 lightPos(0.0f, 40.0f, 30.0f);
glm::vec3 lightPos(2.0f, 2.5f, 15.0f);

Player ski;


int main(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Solar System with skybox", NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // 添加相关窗口交互
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // glfwSetMouseButtonCallback(window,mouse_button_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // 鼠标移动时，光标不可见，并且保持在窗口中

    if(glewInit() != GLEW_OK){
        std::cout << "Failed to initialize GLEW" << std::endl;
        exit(-1);
    }
    glEnable(GL_DEPTH_TEST); // 启用深度测试
    
    Shader lightingShader("shaders/lightingShader.vs", "shaders/lightingShader.fs");
    Shader lightCubeShader("shaders/light_cube.vs", "shaders/light_cube.fs");
    Shader skyboxshader("shaders/skybox.vs","shaders/skybox.fs");
    Shader groundShader("shaders/lightingShader.vs","shaders/lightingShader.fs");
    Shader treeShader("shaders/tree/tree.vs","shaders/tree/tree.fs");
    Shader simpleDepthShader("shaders/depth.vs","shaders/depth.fs");
    Shader shadow("shaders/Shadow.vs","shaders/Shadow.fs");
    Shader debugDepthQuad("shaders/debug_quad.vs", "shaders/debug_quad_depth.fs");
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    float skyboxVertices[] = {
        // positions          
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
    std::vector<glm::vec3> bushpose = {
        glm::vec3(-1.5f, -0.9f, 20.0f), 
        glm::vec3(3.2f, -0.9f, 55.0f),
        glm::vec3(-3.8f, -0.9f, 80.0f)
    };

    std::vector<glm::vec3> rockpose = {
        glm::vec3(-2.5f, -0.9f, 15.0f), 
        glm::vec3(2.1f, -0.9f, 35.0f),
        glm::vec3(0.5f, -0.9f, 48.0f),
        glm::vec3(1.2f, -0.9f, 62.0f),
        glm::vec3(-1.9f, -0.9f, 90.0f)
    };

    std::vector<glm::vec3> treepose = {
        glm::vec3(5.0f, -0.9f, 10.0f), 
        glm::vec3(5.0f, -0.9f, 30.0f),
        glm::vec3(5.0f, -0.9f, 50.0f),
        glm::vec3(5.0f, -0.9f, 70.0f),
        glm::vec3(4.0f, -0.9f, 90.0f),
        glm::vec3(-5.0f, -0.9f, 11.0f), 
        glm::vec3(-5.0f, -0.9f, 31.0f),
        glm::vec3(-5.0f, -0.9f, 51.0f),
        glm::vec3(-5.0f, -0.9f, 71.0f),
        glm::vec3(-4.0f, -0.9f, -90.0f),
    };
    //glEnable(GL_DEPTH_TEST);


    // configure depth map FBO
    // -----------------------
    // configure depth map FBO
    // -----------------------
    const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    // create depth texture
    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    shadow.use();
    shadow.setInt("diffuseTexture", 0);
    shadow.setInt("shadowMap", 1);
    debugDepthQuad.use();
    debugDepthQuad.setInt("depthMap", 0);

    
    unsigned int VBO,VBO_sky, cubeVAO,skyboxVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &VBO_sky);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    glBindVertexArray(cubeVAO);

   // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_sky);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    
    glBindVertexArray(skyboxVAO);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_sky);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    // we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // initialize ground
    GroundSurface ground = GroundSurface(200.0f,200.0f,-200.0f,-200.0f,-1.0f);
    GroundSurface finishLine = GroundSurface(15.0f,110.0f,-15.0f,100.0f,-0.99f);

    Firework* redFire    = new Firework(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(1.0f,0.0f,0.0f),glm::vec3(0.1f,1.0f,0.0f),5.0f,3.0f);
    Firework* yellowFire = new Firework(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(1.0f,1.0f,0.0f),glm::vec3(0.1f,1.0f,0.0f),5.0f,3.0f);
    Firework* orangeFire = new Firework(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(1.0f,0.5f,0.0f),glm::vec3(0.1f,1.0f,0.0f),5.0f,3.0f);
    Firework* goldFire   = new Firework(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(1.0f,0.84f,0.0f),glm::vec3(0.1f,1.0f,0.0f),5.0f,3.0f);
    Firework* redFire_1  = new Firework(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(1.0f,0.0f,0.0f),glm::vec3(0.1f,1.0f,0.0f),5.0f,3.0f);
    Firework* yellowFire_1 = new Firework(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(1.0f,1.0f,0.0f),glm::vec3(0.1f,1.0f,0.0f),5.0f,3.0f);
    Firework* orangeFire_1 = new Firework(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(1.0f,0.5f,0.0f),glm::vec3(0.1f,1.0f,0.0f),5.0f,3.0f);
    Firework* goldFire_1   = new Firework(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(1.0f,0.84f,0.0f),glm::vec3(0.1f,1.0f,0.0f),5.0f,3.0f);

    // initialize snow
    SnowStorm* Snow_0 = new SnowStorm(glm::vec3(8.0f,5.0f,0.0f),glm::vec3(0.9f,0.9f,0.9f),glm::vec3(-1.0f,0.0f,0.0f),3.0f,3.0f);
    SnowStorm* Snow_1 = new SnowStorm(glm::vec3(10.0f,8.0f,5.0f),glm::vec3(0.9f,0.9f,0.9f),glm::vec3(-1.0f,0.0f,0.0f),3.0f,3.0f);
    SnowStorm* Snow_2 = new SnowStorm(glm::vec3(10.0f,8.0f,10.0f),glm::vec3(0.9f,0.9f,0.9f),glm::vec3(-1.0f,0.0f,0.0f),3.0f,3.0f);
    SnowStorm* Snow_3 = new SnowStorm(glm::vec3(10.0f,10.0f,15.0f),glm::vec3(0.9f,0.9f,0.9f),glm::vec3(-1.0f,0.0f,0.0f),3.0f,3.0f);
    SnowStorm* Snow_4 = new SnowStorm(glm::vec3(10.0f,20.0f,30.0f),glm::vec3(0.9f,0.9f,0.9f),glm::vec3(-1.0f,0.0f,0.0f),3.0f,3.0f);
    SnowStorm* Snow_5 = new SnowStorm(glm::vec3(10.0f,20.0f,50.0f),glm::vec3(0.9f,0.9f,0.9f),glm::vec3(-1.0f,0.0f,0.0f),3.0f,3.0f);

    SnowStorm* Snow_6 = new SnowStorm(glm::vec3(8.0f,3.0f,0.0f),glm::vec3(0.9f,0.9f,0.9f),glm::vec3(1.0f,0.0f,0.0f),3.0f,3.0f);
    SnowStorm* Snow_7 = new SnowStorm(glm::vec3(12.0f,5.0f,5.0f),glm::vec3(0.9f,0.9f,0.9f),glm::vec3(1.0f,0.0f,0.0f),3.0f,3.0f);
    SnowStorm* Snow_8 = new SnowStorm(glm::vec3(12.0f,5.0f,10.0f),glm::vec3(0.9f,0.9f,0.9f),glm::vec3(1.0f,0.0f,0.0f),3.0f,3.0f);
    SnowStorm* Snow_9 = new SnowStorm(glm::vec3(15.0f,7.0f,15.0f),glm::vec3(0.9f,0.9f,0.9f),glm::vec3(1.0f,0.0f,0.0f),3.0f,3.0f);
    SnowStorm* Snow_10 = new SnowStorm(glm::vec3(15.0f,10.0f,30.0f),glm::vec3(0.9f,0.9f,0.9f),glm::vec3(1.0f,0.0f,0.0f),3.0f,3.0f);



    // initialize L-system tree & rock & bush
    LSystem tree;
    bush bush(1.5, glm::vec3(1.0f, 1.0f, 1.0f));
    Rock rock(0.9, glm::vec3(1.0f, 1.0f, 1.0f));

    std::vector<std::string> faces
    {
        "texture/skybox2/right.png",
        "texture/skybox2/left.png",
        "texture/skybox2/top.png",
        "texture/skybox2/bottom.png",
        "texture/skybox2/back.png",
        "texture/skybox2/front.png"
    };
    unsigned int cubemapTexture = loadCubemap(faces); 
    // 导入地面的纹理
    unsigned int groundTexture = load_texture("texture/skybox2/surface_new.jpg");
    unsigned int finishLineTexture = load_texture("texture/skybox2/finish.jpg");
    unsigned int trunkTexture = load_texture("texture/Tree_Rock/tree.jpg");
    unsigned int rockTexture = load_texture("texture/Tree_Rock/Rock.jpg");
    unsigned int bushTexture = load_texture("texture/Tree_Rock/bush.jpg");
    unsigned int sunTexture = load_texture("texture/Tree_Rock/8k_sun.jpg");

    // 设置烟花着色器
    Shader FireworkShader("shaders/firework/firework.vs","shaders/firework/firework.fs","shaders/firework/firework.gs");
    Shader SnowShader("shaders/snow/snow.vs","shaders/snow/snow.fs","shaders/snow/snow.gs");
    

    // 导入滑雪人的obj文件
    Model skier1 = Model("skier.obj");
    Model skier2 = Model("skier2.obj");
    Shader skierShader("shaders/skier/skier.vs","shaders/skier/skier.fs");
    float car_angle = 0.0f;
    GLint pose = 0;
    while(!glfwWindowShouldClose(window)){
        
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        processKeyboardInput(window);

        lightPos.z = ski.Position.z + 10;
        //lightPos.x = sin(glfwGetTime()) * 3.0f;
        //lightPos.z = cos(glfwGetTime()) * 2.0f;
        lightPos.y = 9.0f;
        glClearColor(0.1f,0.1f,0.1f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        

        // 1. render depth of scene to texture (from light's perspective)
        // --------------------------------------------------------------
        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        float near_plane = 0.1f, far_plane = 20.0f;
        //lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
        lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, near_plane, far_plane);
        lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;
        // render scene from light's point of view
        simpleDepthShader.use();
        simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), screenWidth / screenHeight, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, groundTexture);

        for(int i = 0; i<treepose.size(); i++){
            if(0.0f < treepose[i][2] - ski.Position.z && treepose[i][2] - ski.Position.z < 30.0f)
                tree.generateFractal(simpleDepthShader, projection, view, treepose[i], &trunkTexture, &sunTexture);
        }
        for(int i = 0; i<bushpose.size(); i++){
            glm::mat4 model_bush = glm::mat4(1.0f);
            model_bush = glm::translate(model_bush, bushpose[i]);  
            if(bushpose[i][2] - ski.Position.z < 30.0f && bushpose[i][2] - ski.Position.z > 0.0f)
                bush.draw(simpleDepthShader, projection, view, model_bush, &bushTexture);
        }
        for(int i = 0; i<rockpose.size(); i++){
            glm::mat4 model_rock = glm::mat4(1.0f);
            model_rock = glm::translate(model_rock, rockpose[i]);
            if(rockpose[i][2] - ski.Position.z < 30.0f && rockpose[i][2] - ski.Position.z > 0.0f)
                rock.draw(simpleDepthShader, projection, view, model_rock, &rockTexture);
        }
        GLint poseAlt = 30.0f / ski.Speed;
        if (poseAlt < 15) poseAlt = 15;
        pose = (pose + 1) % poseAlt;
        Model skier = IsSuccess || IsFree || pose < poseAlt / 2 ? Model("skier.obj") : Model("skier2.obj");
        glm::mat4 model_ski=glm::mat4(1.0f);
        model_ski = glm::translate(model_ski, ski.Position);
        skier.use();
        skier.draw(simpleDepthShader,projection,view,model_ski);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // reset viewport
        glViewport(0, 0, screenWidth, screenHeight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDepthMask(GL_FALSE);
        skyboxshader.use();
        glm::mat4 projection_skybox;
        projection_skybox = glm::perspective(glm::radians(camera.Zoom), screenWidth / screenHeight, 0.1f, 100.0f); 
        int projectionLoc_s = glGetUniformLocation(skyboxshader.ID, "projection");
        glUniformMatrix4fv(projectionLoc_s, 1, GL_FALSE, glm::value_ptr(projection_skybox));

        glm::mat4 view_s = glm::mat4(glm::mat3(camera.GetViewMatrix())); ; 
        int viewLoc_s = glGetUniformLocation(skyboxshader.ID,"view");
        glUniformMatrix4fv(viewLoc_s, 1, GL_FALSE, glm::value_ptr(view_s));
        // ... set view and projection matrix
        glBindVertexArray(skyboxVAO);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthMask(GL_TRUE);


        // debugDepthQuad.use();
        // debugDepthQuad.setFloat("near_plane", near_plane);
        // debugDepthQuad.setFloat("far_plane", far_plane);
        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, depthMap);
        // renderQuad();

        // draw ground
        glm::mat4 model_ground = glm::mat4(1.0f);
        // set ground shader uniforms
        shadow.use();
        shadow.setVec3("material.ambient",  glm::vec3(0.8f, 0.8f, 0.8f));
        shadow.setVec3("material.diffuse",  glm::vec3(0.8f, 0.8f, 0.8f));
        shadow.setVec3("material.specular", glm::vec3(0.1f, 0.1f, 0.1f));
        shadow.setFloat("material.shininess", 30.0f);
        shadow.setVec3("light.ambient",  glm::vec3(0.25f, 0.25f, 0.25f));
        shadow.setVec3("light.diffuse",  glm::vec3(0.5f, 0.5f, 0.5f)); 
        shadow.setVec3("light.specular", glm::vec3(0.1f, 0.1f, 0.1f)); 
        shadow.setVec3("light.position", lightPos);
        shadow.setVec3("lightColor",  glm::vec3(1.0f,1.0f,1.0f));
        shadow.setMat4("projection", projection);
        shadow.setMat4("view", view);
        // set light uniforms
        shadow.setVec3("viewPos", camera.Position);
        shadow.setVec3("lightPos", lightPos);
        shadow.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, groundTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);

        ground.draw(shadow,projection,view,model_ground,groundTexture);
        finishLine.draw(shadow,projection,view,model_ground,finishLineTexture);

        // draw tree & rock & bush
        shadow.use();
        shadow.setVec3("material.ambient",  glm::vec3(2.0f, 2.0f, 2.0f));
        shadow.setVec3("material.diffuse",  glm::vec3(0.5f, 0.5f, 0.5f));
        shadow.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
        shadow.setFloat("material.shininess", 64.0f);
        shadow.setVec3("light.ambient",  glm::vec3(0.3f, 0.3f, 0.3f));
        shadow.setVec3("light.diffuse",  glm::vec3(1.0f, 1.0f, 1.0f)); 
        shadow.setVec3("light.specular", glm::vec3(3.0f, 3.0f, 3.0f)); 
        shadow.setVec3("light.position", lightPos);
        shadow.setVec3("lightColor",  glm::vec3(1.0f,1.0f,1.0f));
        for(int i = 0; i<treepose.size(); i++){
            if(0.0f < treepose[i][2] - ski.Position.z && treepose[i][2] - ski.Position.z < 30.0f)
                tree.generateFractal(shadow, projection, view, treepose[i], &trunkTexture, &sunTexture);
        }
        for(int i = 0; i<bushpose.size(); i++){
            glm::mat4 model_bush = glm::mat4(1.0f);
            model_bush = glm::translate(model_bush, bushpose[i]);  
            if(bushpose[i][2] - ski.Position.z < 30.0f && bushpose[i][2] - ski.Position.z > 0.0f)
                bush.draw(shadow, projection, view, model_bush, &bushTexture);
        }
        for(int i = 0; i<rockpose.size(); i++){
            glm::mat4 model_rock = glm::mat4(1.0f);
            model_rock = glm::translate(model_rock, rockpose[i]);
            if(rockpose[i][2] - ski.Position.z < 30.0f && rockpose[i][2] - ski.Position.z > 0.0f)
                rock.draw(shadow, projection, view, model_rock, &rockTexture);
        }

        if(IsSuccess == false && IsFree == false){
            ski.Position = ski.Position + glm::vec3(ski.Front.x*deltaTime*ski.Speed, 0.0f, ski.Front.z*deltaTime*ski.Speed);
        }
        glm::mat4 model=glm::mat4(1.0f);
        if(IsFree == false){
            camera.Front = glm::vec3(0, -0.2f, 1.0f);
            camera.Position = ski.Position + glm::vec3(0, 3.0f, -5.0f);
        }
        model = glm::translate(model, ski.Position);
        skierShader.use();    
        // set light uniforms
        skierShader.setVec3("objectColor", glm::vec3(0.9f, 0.4f, 0.4f));
        skierShader.setVec3("lightColor",  glm::vec3(1.0f, 1.0f, 1.0f));
        skierShader.setVec3("lightPos", lightPos);
        skierShader.setVec3("viewPos", camera.Position);

        
        
        skier.use();
        skier.draw(skierShader,projection,view,model_ski);

        // 碰撞检测
        for (int i = 0; i < bushpose.size(); ++i)
            hitbox(bushpose[i], 2.0f);
        for (int i = 0; i < rockpose.size(); ++i)
            hitbox(rockpose[i], 2.0f);
        for (int i = 0; i < treepose.size(); ++i)
            hitbox(treepose[i], 1.0f);


        // 绘制雪
        glDepthMask(false);
        glm::mat4 snow_model = glm::mat4(1.0f);
        glm::mat4 snow_model_update_1 = glm::translate(snow_model,glm::vec3(0.0f, 0.0f, 40.0f));
        glm::mat4 snow_model_update_2 = glm::translate(snow_model,glm::vec3(0.0f, 0.0f, 70.0f));
        if(ski.Position.z > 30.0f && RestartSnow_1 == false){
            Snow_0->updateBirthTime();
            Snow_1->updateBirthTime();
            Snow_2->updateBirthTime();
            Snow_3->updateBirthTime();
            Snow_4->updateBirthTime();
            Snow_5->updateBirthTime();
            Snow_6->updateBirthTime();
            Snow_7->updateBirthTime();
            Snow_8->updateBirthTime();
            Snow_9->updateBirthTime();
            Snow_10->updateBirthTime();
            RestartSnow_1 = true;
        }
        else if(ski.Position.z > 60.0f && RestartSnow_1 == true && RestartSnow_2 == false){
            Snow_0->updateBirthTime();
            Snow_1->updateBirthTime();
            Snow_2->updateBirthTime();
            Snow_3->updateBirthTime();
            Snow_4->updateBirthTime();
            Snow_5->updateBirthTime();
            Snow_6->updateBirthTime();
            Snow_7->updateBirthTime();
            Snow_8->updateBirthTime();
            Snow_9->updateBirthTime();
            Snow_10->updateBirthTime();
            RestartSnow_2 = true;
        }
        if(RestartSnow_1 == false){
            Snow_0->drawFlow(SnowShader,projection,view,snow_model);
            Snow_1->drawFlow(SnowShader,projection,view,snow_model);
            Snow_2->drawFlow(SnowShader,projection,view,snow_model);
            Snow_3->drawFlow(SnowShader,projection,view,snow_model);
            Snow_4->drawFlow(SnowShader,projection,view,snow_model);
            Snow_5->drawFlow(SnowShader,projection,view,snow_model);
            Snow_6->drawFlow(SnowShader,projection,view,snow_model);
            Snow_7->drawFlow(SnowShader,projection,view,snow_model);
            Snow_8->drawFlow(SnowShader,projection,view,snow_model);
            Snow_9->drawFlow(SnowShader,projection,view,snow_model);
            Snow_10->drawFlow(SnowShader,projection,view,snow_model);
        }else if(RestartSnow_1 == true && RestartSnow_2 == false){
            Snow_0->drawFlow(SnowShader,projection,view,snow_model_update_1);
            Snow_1->drawFlow(SnowShader,projection,view,snow_model_update_1);
            Snow_2->drawFlow(SnowShader,projection,view,snow_model_update_1);
            Snow_3->drawFlow(SnowShader,projection,view,snow_model_update_1);
            Snow_4->drawFlow(SnowShader,projection,view,snow_model_update_1);
            Snow_5->drawFlow(SnowShader,projection,view,snow_model_update_1);
            Snow_6->drawFlow(SnowShader,projection,view,snow_model_update_1);
            Snow_7->drawFlow(SnowShader,projection,view,snow_model_update_1);
            Snow_8->drawFlow(SnowShader,projection,view,snow_model_update_1);
            Snow_9->drawFlow(SnowShader,projection,view,snow_model_update_1);
            Snow_10->drawFlow(SnowShader,projection,view,snow_model_update_1);
        }else{
            Snow_0->drawFlow(SnowShader,projection,view,snow_model_update_2);
            Snow_1->drawFlow(SnowShader,projection,view,snow_model_update_2);
            Snow_2->drawFlow(SnowShader,projection,view,snow_model_update_2);
            Snow_3->drawFlow(SnowShader,projection,view,snow_model_update_2);
            Snow_4->drawFlow(SnowShader,projection,view,snow_model_update_2);
            Snow_5->drawFlow(SnowShader,projection,view,snow_model_update_2);
            Snow_6->drawFlow(SnowShader,projection,view,snow_model_update_2);
            Snow_7->drawFlow(SnowShader,projection,view,snow_model_update_2);
            Snow_8->drawFlow(SnowShader,projection,view,snow_model_update_2);
            Snow_9->drawFlow(SnowShader,projection,view,snow_model_update_2);
            Snow_10->drawFlow(SnowShader,projection,view,snow_model_update_2);
        }
        glDepthMask(true);
        // 如果car_pos对应的z轴坐标大于100.0f，则放烟花
        if(ski.Position.z > 100.0f && !IsSuccess){
            redFire->update();
            yellowFire->update();
            orangeFire->update();
            goldFire->update();
            redFire_1->update();
            yellowFire_1->update();
            orangeFire_1->update();
            goldFire_1->update();
            IsSuccess = true;
        }
        if(IsSuccess == true){
            glm::mat4 FireworkModel = glm::mat4(1.0f);
            FireworkModel = glm::translate(FireworkModel, glm::vec3(12.0f, 0.0f, 125.0f));
            redFire->drawFlow(FireworkShader,projection,view,FireworkModel);
            yellowFire->drawFlow(FireworkShader,projection,view,FireworkModel);
            orangeFire->drawFlow(FireworkShader,projection,view,FireworkModel);
            goldFire->drawFlow(FireworkShader,projection,view,FireworkModel);
            FireworkModel = glm::translate(FireworkModel, glm::vec3(-24.0f, 0.0f, 0.0f));
            redFire_1->drawFlow(FireworkShader,projection,view,FireworkModel);
            yellowFire_1->drawFlow(FireworkShader,projection,view,FireworkModel);
            orangeFire_1->drawFlow(FireworkShader,projection,view,FireworkModel);
            goldFire_1->drawFlow(FireworkShader,projection,view,FireworkModel); 
        }
        
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
void processKeyboardInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
    float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        if(IsFree)
            camera.ProcessKeyboard(FORWARD, deltaTime);
        else
            ski.Speed += 0.1;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        if(IsFree)
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        else
            ski.Speed = (ski.Speed - 0.1) > 0 ? ski.Speed - 0.1 : 0;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        if(IsFree)
            camera.ProcessKeyboard(LEFT, deltaTime);
        else
            ski.Position += glm::vec3(cameraSpeed * 0.5, 0.0f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        if(IsFree)
            camera.ProcessKeyboard(RIGHT, deltaTime);
        else
            ski.Position += glm::vec3(-cameraSpeed * 0.5, 0.0f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS){
        IsFree = !IsFree;
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    if(firstMouse) // 这个bool变量初始时是设定为true的
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    if(IsFree)
        camera.ProcessMouseMovement(xoffset,yoffset);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    camera.ProcessMouseScroll(yoffset);
}

void hitbox(glm::vec3 barrier, float dist){
    if((ski.Position.x < barrier.x + dist && ski.Position.x > barrier.x - dist) && (ski.Position.z < barrier.z + dist && ski.Position.z > barrier.z - dist)){
        ski.Position = glm::vec3(0, -1.0f, 0);
        RestartSnow_1 = false;
        RestartSnow_2 = false;
        IsSuccess = false;
        IsFree = false;
    }
}

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}