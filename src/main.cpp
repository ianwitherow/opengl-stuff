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

using namespace std;

GLfloat vertices[] {
    // X      Y     Z     R     G     B     U     V
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // Right side
    0.5f , -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    0.5f ,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    0.5f ,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,

    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // Left side
    0.5f , -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    0.5f ,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    0.5f ,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // Back
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

    0.5f ,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // Front
    0.5f ,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    0.5f , -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    0.5f , -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    0.5f , -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    0.5f ,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // Bottom
    0.5f , -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    0.5f , -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    0.5f , -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,

    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // Top
    0.5f ,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    0.5f ,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    0.5f ,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};

bool paused = false;
double pausedMouseX;
double pausedMouseY;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
glm::vec4 bgColor = glm::vec4(0.2f, 0.3f, 0.4f, 0.0f);

// Camera
glm::vec3 cameraPos = glm::vec3(5.0f, 0.f, 0.f);
glm::vec3 cameraFront = glm::vec3(-1.f, 0.f, 0.f);
glm::vec3 cameraUp = glm::vec3(0.f, 1.f, 0.f);
float cameraSpeed = 0.1f;
float Yaw = -180.0f;
float Pitch = 0.0f;
float mouseSensitivity = 0.1f;

// Mouse stuff
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = false;

// timing
float deltaTime = 0.0f;
float lastframe = 0.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow *window);

void init() {
    // Get those vertices up in they

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

}

void renderBg() {
	glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void render() {
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

int main(int argc, char *argv[]) {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
	#endif

		GLFWwindow* window = glfwCreateWindow(800, 600, "Moo!", NULL, NULL);
		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return -1;
		}
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetMouseButtonCallback(window, mouse_button_callback);
		glfwSetScrollCallback(window, scroll_callback);
		glfwSetKeyCallback(window, key_callback);

    // tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(window, SCR_WIDTH / 2, SCR_HEIGHT / 2);

    glewExperimental = GL_TRUE;
    glewInit();

    init();
    printf("Moo 2!\n");

    glEnable(GL_DEPTH_TEST);

    GLuint shaderProgram;
    initShaders("src/minimalTex.vert", "src/normalTexture.frag", shaderProgram);

    glUseProgram(shaderProgram);

    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    __glewVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), 0);

    GLint colorAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colorAttrib);
    __glewVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));

    GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
    __glewVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(texAttrib);

    GLuint tex;
    glGenTextures(1, &tex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);

    sf::Image image;

    image.loadFromFile("src/dirt.jpg");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getSize().x, image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
    glUniform1i(glGetUniformLocation(shaderProgram, "theTexture"), 0);


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    

    GLint e = glGetError();

    printf("%i\r", e);

    GLint uniModel = glGetUniformLocation(shaderProgram, "model");
    GLint uniView = glGetUniformLocation(shaderProgram, "view");
    GLint uniProj = glGetUniformLocation(shaderProgram, "proj");

    glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 1.0f, 100.0f);

    glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));
    
    auto t_start = chrono::high_resolution_clock::now();

    while (!glfwWindowShouldClose(window)) {
		processInput(window);

		//printf("%f %f %f\n", cameraFront.x, cameraFront.y, cameraFront.z);

        auto t_now = chrono::high_resolution_clock::now();
        float time = chrono::duration_cast<chrono::duration<float>>(t_now - t_start).count();

        glm::mat4 model;
        //model = glm::rotate(model, time * glm::radians(-180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

        glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

        glm::mat4 view = glm::lookAt(
            cameraPos,
            cameraPos + cameraFront,
            cameraUp
        );

        glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
        
        // Draw objects
        renderBg();
        render();

        // Switch to the front buffer
		glfwSwapBuffers(window);
		glfwPollEvents();

    }

    return 0;
}

void processInput(GLFWwindow *window) {
	if (paused) {
		return;
	}

	bool colemak = false;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPos += cameraSpeed * cameraFront;
	}
	if ((colemak && glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS || !colemak && glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)) {
		cameraPos -= cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if ((colemak && glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || !colemak && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		cameraPos.y += cameraSpeed;
	}
	if ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)) {
		cameraPos.y -= cameraSpeed;
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (paused) { return; }

	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = ypos - lastY;

	lastX = xpos;
	lastY = ypos;

	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;
	yoffset = -yoffset;

	Yaw += xoffset;
	Pitch += yoffset;

	cameraFront = glm::normalize(glm::vec3(
		cos(glm::radians(Yaw)) * cos(glm::radians(Pitch)),
		sin(glm::radians(Pitch)),
		sin(glm::radians(Yaw)) * cos(glm::radians(Pitch))
	));

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		paused = !paused;
		if (paused) {
			printf("Paused\n");
			// Save mouse pos
			glfwGetCursorPos(window, &pausedMouseX, &pausedMouseY);

			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else {
			printf("Un-paused\n");
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			glfwSetCursorPos(window, pausedMouseX, pausedMouseY);
			lastX = (float)pausedMouseX;
			lastY = (float)pausedMouseY;
		}
		printf("Esc");
	}

	if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	// Movement. If paused, don't do this stuff.
	if (paused) { return; }

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
