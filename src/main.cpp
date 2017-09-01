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
#include "Cube.h"
#include "Skybox.h"

using namespace std;

vector<string> skyboxImages = {
    ResourceFile("skybox_right.png"),
    ResourceFile("skybox_left.png"),
    ResourceFile("skybox_top.png"),
    ResourceFile("skybox_bottom.png"),
    ResourceFile("skybox_back.png"),
    ResourceFile("skybox_front.png")
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
GLuint cubeTexId;
GLuint waterTexId;
Shader cubeShader;
Shader waterShader;

Skybox skybox = Skybox();

Game game = Game(800, 600);

void init() {
    // Get those vertices up in they

	Cube::Init();

	Cube::SetAttributes(cubeShader);
	Cube::SetAttributes(waterShader);

    // Skybox
	skybox.init(skyboxImages);
}

void renderBg() {
	glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void render() {
    auto view = game.player.camera.getView();

    glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 1.0f, 1000.0f);

    glm::mat4 model;

	cubeShader.use();

	cubeShader.setMat4("model", model);
	cubeShader.setMat4("view", view);
	cubeShader.setMat4("proj", proj);

    glDepthMask(GL_TRUE);


	for (int x = 0; x < 1600; x+=10) {
		float myX = (float)x / (float)10;
		for (int z = 0; z < 1000; z += 10) {
			float myZ = (float)z / (float)10;

			auto cube = Cube(glm::vec3(myX, 0.f, myZ), cubeShader);
			cube.Render(cubeTexId);
		}
	}

	waterShader.use();

	waterShader.setMat4("model", model);
	waterShader.setMat4("view", view);
	waterShader.setMat4("proj", proj);

	float time = (sin((float)glfwGetTime() * 4.0f) + 1.0f) / 2.0f;
	waterShader.setFloat("time", time);


	// draw water blocks
	for (int x = 0; x < 100; x+=10) {
		float myX = (float)x / (float)10;
		for (int z = 0; z < 1000; z += 10) {
			float myZ = (float)z / (float)10;

			auto cube = Cube(glm::vec3(-myX - 1, 0.f, myZ), waterShader);
			cube.Render(waterTexId);
		}
	}

    glBindVertexArray(0);

	skybox.Render(view, proj);

}

int main(int argc, char *argv[]) {
    Player player = Player();

    // Set up camera. Initial state:
    glm::vec3 cameraPosition = glm::vec3(5.0f, player.height, 3.f);
    glm::vec3 cameraFront = glm::vec3(-1.f, 0.f, 0.f);
	// Y is up
    glm::vec3 cameraUp = glm::vec3(0.f, 1.f, 0.f);

    Camera camera = Camera(cameraPosition, cameraFront, cameraUp);

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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	cubeShader = Shader(ResourceFile("minimalTex.vert").c_str(), ResourceFile("normalTexture.frag").c_str());
	waterShader = Shader(ResourceFile("minimalTex.vert").c_str(), ResourceFile("water.frag").c_str());

    // Load vertices and setup attributes
    init();

    // Configure the grass texture
    glGenTextures(1, &cubeTexId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cubeTexId);

    sf::Image image;

    image.loadFromFile(ResourceFile("DirtGrass.png"));
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
    image.loadFromFile(ResourceFile("water.jpg"));

    // OSX
    //image.loadFromFile("DirtGrass.png");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getSize().x, image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
	//waterShader.setInt("theTexture", 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);



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
        //printf("Paused\n");
		return;
	}

	bool colemak = true;

    // TODO: Move stuff to camera or game.player class
    // Forward
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        game.player.camera.moveForward(game.freeFly);
        //printf("W!\n");
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
