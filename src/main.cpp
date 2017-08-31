//#include "moo_includes.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <GL/glut.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
//#include <GL/GL.h>
#else
#endif

#include <SFML/Graphics.hpp>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <math.h>

#include <chrono>

#include "helpers.hpp"
#include "Shader.h"

#include "Camera.h"
#include "Player.h"
#include "Game.h"

using namespace std;

GLfloat vertices[] {
    // X      Y     Z     R     G     B     U     V
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.32f , 1.0f     , // Right side
    0.5f , -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f  , 1.0f     ,
    0.5f ,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f  , 0.0f     ,
    0.5f ,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f  , 0.0f     ,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.32f , 0.0f     ,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.32f , 1.0f     ,

    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  0.32f , 1.0f     , // Left side
    0.5f , -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  0.0f  , 1.0f     ,
    0.5f ,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  0.0f  , 0.0f     ,
    0.5f ,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  0.0f  , 0.0f     ,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  0.32f , 0.0f     ,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  0.32f , 1.0f     ,

    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  0.0f  ,   0.0f   , // Back
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.32f ,  0.0f    ,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.32f ,  1.0f    ,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.32f ,  1.0f    ,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  0.0f  ,   1.0f   ,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  0.0f  ,   0.0f   ,

    0.5f ,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f   , 0.0f     , // Front
    0.5f ,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.32f  , 0.0f     ,
    0.5f , -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.32f  , 1.0f     ,
    0.5f , -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.32f  , 1.0f     ,
    0.5f , -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f   , 1.0f     ,
    0.5f ,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f   , 0.0f     ,

	 -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f,    1.0f, //Bottom
	 0.5f , -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.67f ,  1.0f, 
	 0.5f , -0.5f, 0.5f , 1.0f, 1.0f, 1.0f, 0.67f ,  0.0f, 
	 0.5f , -0.5f, 0.5f , 1.0f, 1.0f, 1.0f, 0.67f ,  0.0f, 
	 -0.5f, -0.5f, 0.5f , 1.0f, 1.0f, 1.0f, 1.0f,    0.0f, 
	 -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f,    1.0f, 

    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.66f  ,   1.0f   , // Top
    0.5f ,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.34f  ,   1.0f   ,
    0.5f ,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.34f  ,   0.0f   ,
    0.5f ,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.34f  ,   0.0f   ,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.66f  ,   0.0f   ,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.66f  ,   1.0f   
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

vector<string> skyboxImages = {
    "skybox_right.png",
    "skybox_left.png",
    "skybox_top.png",
    "skybox_bottom.png",
    "skybox_back.png",
    "skybox_front.png"
};

// Game state stuff


glm::vec4 bgColor = glm::vec4(0.2f, 0.3f, 0.4f, 0.0f);

// Camera

// timing
float deltaTime = 0.0f;
float lastframe = 0.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow *window);

// Rendering
GLuint cubeVao;
GLuint skyboxVao;
GLuint skyboxTexId;
GLuint cubeTexId;
GLuint waterTexId;
Shader cubeShader;
Shader waterShader;
Shader skyboxShader;

Game game = Game(800, 600);

void init() {
    // Get those vertices up in they

    glGenVertexArrays(1, &cubeVao);
    glBindVertexArray(cubeVao);

    GLuint cubeVbo;
    glGenBuffers(1, &cubeVbo);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLint cubePosAttrib = glGetAttribLocation(cubeShader.ID, "position");
    glEnableVertexAttribArray(cubePosAttrib);
    __glewVertexAttribPointer(cubePosAttrib, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), 0);

    GLint colorAttrib = glGetAttribLocation(cubeShader.ID, "color");
    glEnableVertexAttribArray(colorAttrib);
    __glewVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));

    GLint texAttrib = glGetAttribLocation(cubeShader.ID, "texcoord");
    __glewVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(texAttrib);

    GLint waterPosAttrib = glGetAttribLocation(waterShader.ID, "position");
    glEnableVertexAttribArray(waterPosAttrib);
    __glewVertexAttribPointer(waterPosAttrib, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), 0);

    GLint waterColorAttrib = glGetAttribLocation(waterShader.ID, "color");
    glEnableVertexAttribArray(waterColorAttrib);
    __glewVertexAttribPointer(waterColorAttrib, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));

    GLint waterTexAttrib = glGetAttribLocation(waterShader.ID, "texcoord");
    __glewVertexAttribPointer(waterTexAttrib, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(waterTexAttrib);

    // Skybox
    glGenVertexArrays(1, &skyboxVao);
    glBindVertexArray(skyboxVao);

    GLuint skyboxVbo;
    glGenBuffers(1, &skyboxVbo);

    glBindBuffer(GL_ARRAY_BUFFER, skyboxVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

    GLint skyboxPosAttrib = glGetAttribLocation(skyboxShader.ID, "position");
    glEnableVertexAttribArray(skyboxPosAttrib);
    __glewVertexAttribPointer(skyboxPosAttrib, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);

}

void renderBg() {
	glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void render() {
    glm::mat3 view = game.player.camera.getView();

    glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 1.0f, 1000.0f);

    glm::mat4 model;

	cubeShader.use();

	cubeShader.setMat4("model", model);
	cubeShader.setMat4("view", view);
	cubeShader.setMat4("proj", proj);

    glDepthMask(GL_TRUE);

    glBindVertexArray(cubeVao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cubeTexId);

	for (int x = 0; x < 1600; x+=10) {
		float myX = (float)x / (float)10;
		for (int z = 0; z < 1000; z += 10) {
			float myZ = (float)z / (float)10;

			glm::mat4 mdl = glm::translate(glm::mat4(1.f), glm::vec3(myX, 0.f, myZ));
			cubeShader.setMat4("model", mdl);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	}

	waterShader.use();

	waterShader.setMat4("model", model);
	waterShader.setMat4("view", view);
	waterShader.setMat4("proj", proj);

	float time = (sin((float)glfwGetTime() * 4.0f) + 1.0f) / 2.0f;
	waterShader.setFloat("time", time);

	glBindTexture(GL_TEXTURE_2D, waterTexId);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// draw water blocks
	for (int x = 0; x < 100; x+=10) {
		float myX = (float)x / (float)10;
		for (int z = 0; z < 1000; z += 10) {
			float myZ = (float)z / (float)10;

			glm::mat4 mdl = glm::translate(glm::mat4(1.f), glm::vec3(-myX -1, 0.f, myZ));
			waterShader.setMat4("model", mdl);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	}

    glBindVertexArray(0);

    glDepthFunc(GL_LEQUAL);
    skyboxShader.use();

    glm::mat4 skyboxView = glm::mat4(glm::mat3(view));

	skyboxShader.setMat4("proj", proj);
	skyboxShader.setMat4("view", skyboxView);

    glBindVertexArray(skyboxVao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexId);

    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // set depth function back to default

}

int main(int argc, char *argv[]) {

    // Set up camera. Initial state:
    glm::vec3 cameraPosition = glm::vec3(5.0f, 2.5f, 3.f);
    glm::vec3 cameraFront = glm::vec3(-1.f, 0.f, 0.f);
    glm::vec3 cameraUp = glm::vec3(0.f, 1.f, 0.f);

    // TODO: camera member values are not being updated when modified using game.player.camera.whatever = whatever
    Camera camera = Camera(cameraPosition, cameraFront, cameraUp);

    Player player = Player();
    player.camera = camera;
    game = Game(800, 600);
    game.player = player;
    
    game.createWindow();
    
    glfwSetFramebufferSizeCallback(game.window, framebuffer_size_callback);
    glfwSetCursorPosCallback(game.window, mouse_callback);

    glfwSetMouseButtonCallback(game.window, mouse_button_callback);
    glfwSetScrollCallback(game.window, scroll_callback);
    glfwSetKeyCallback(game.window, key_callback);

    // tell GLFW to capture our mouse
	glfwSetInputMode(game.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Remember initial mouse position so it doesn't get all crazy the first time we move it
    double mouseX, mouseY;
    glfwGetCursorPos(game.window, &mouseX, &mouseY);
    game.player.camera.lastX = (float)mouseX;
    game.player.camera.lastY = (float)mouseY;

    glewExperimental = GL_TRUE;
    glewInit();

    printf("Moo 2!\n");

    glEnable(GL_DEPTH_TEST);

    // Windows
    //initShaders("minimalTex.vert", "normalTexture.frag", cubeShaderProgram);
    // OSX

	cubeShader = Shader("minimalTex.vert", "normalTexture.frag");
	waterShader = Shader("minimalTex.vert", "water.frag");

    skyboxShader = Shader("skybox.vert", "skybox.frag");

    // Load vertices and setup attributes
    init();

    // Configure the grass texture
    glGenTextures(1, &cubeTexId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cubeTexId);

    sf::Image image;

    // Windows
    image.loadFromFile("DirtGrass.png");
    // OSX
    //image.loadFromFile("DirtGrass.png");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getSize().x, image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
	cubeShader.setInt("theTexture", 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Water texture
    glGenTextures(1, &waterTexId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, waterTexId);

    // Windows
    image.loadFromFile("water.jpg");

    // OSX
    //image.loadFromFile("DirtGrass.png");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getSize().x, image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
	//waterShader.setInt("theTexture", 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindVertexArray(skyboxVao);

    // Cube map
    glGenTextures(1, &skyboxTexId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexId);

    for (int i = 0; i < skyboxImages.size(); i++) {
        image.loadFromFile(skyboxImages[i]);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0, GL_RGBA, image.getSize().x, image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	skyboxShader.setInt("skybox", 0);

    GLint e = glGetError();

    printf("%i\r", e);

    auto t_start = chrono::high_resolution_clock::now();

    while (!glfwWindowShouldClose(game.window)) {
		processInput(game.window);

		//printf("%f %f %f\n", cameraFront.x, cameraFront.y, cameraFront.z);

        //model = glm::rotate(model, time * glm::radians(-180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

        // Draw objects
        renderBg();
        render();

        // Switch to the front buffer
		glfwSwapBuffers(game.window);
		glfwPollEvents();

    }

    return 0;
}

void processInput(GLFWwindow *window) {
	if (game.paused) {
        printf("Paused\n");
		return;
	}

	bool colemak = false;

    // TODO: Move stuff to camera or game.player class
    // Forward
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        game.player.camera.moveForward(game.freeFly);
        printf("W!\n");
	}

    // Back
	if ((colemak && glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) || (!colemak && glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)) {
        if (!game.freeFly) {
            // If not in freefly, don't allow moving up or down
            game.player.camera.position -= game.player.camera.speed * glm::vec3(game.player.camera.front.x, 0.f, game.player.camera.front.z);
        } else {
            game.player.camera.position -= game.player.camera.speed * game.player.camera.front;
        }
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		game.player.camera.position -= glm::normalize(glm::cross(game.player.camera.front, game.player.camera.up)) * game.player.camera.speed;
	}
	if ((colemak && glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) || (!colemak && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)) {
		game.player.camera.position += glm::normalize(glm::cross(game.player.camera.front, game.player.camera.up)) * game.player.camera.speed;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		game.player.camera.position.y += game.player.camera.speed;
	}

    // Handle shift
    if (game.freeFly) {
        // In free fly, shift goes down
    	if ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)) {
            game.player.camera.position.y -= game.player.camera.speed;
    	}
    } else {
        // Normal mode; shift crouches
    	if ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)) {
            game.player.camera.position.y = 1.5;
        }
    	if ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)) {
            game.player.camera.position.y = 2.5;
        }
    }

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (game.paused) { return; }

    game.player.camera.handleMouseMove(xpos, ypos);

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    printf("%i\n", key);

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		if (game.paused) {
            game.resume();
		}
		else {
            game.pause();
		}
		printf("Esc");
	}

    // Toggle free fly
	if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
        game.freeFly = !game.freeFly;
    }

	if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

    if (key == GLFW_KEY_MINUS) {
        game.player.camera.speed -= 0.1f;
        if (game.player.camera.speed <= 0.f) {
            game.player.camera.speed = 0.1f;
        }
    }

    if (key == GLFW_KEY_EQUAL) {
        game.player.camera.speed += 0.1f;
    }

}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		printf("%f %f %f\n", r, g, b);
		bgColor = glm::vec4(r, g, b, 1.0f);

	}
}
