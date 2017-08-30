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
    "src/skybox_right.png",
    "src/skybox_left.png",
    "src/skybox_top.png",
    "src/skybox_bottom.png",
    "src/skybox_back.png",
    "src/skybox_front.png"
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

// Rendering
GLuint cubeVao;
GLuint skyboxVao;
GLuint skyboxTexId;
GLuint cubeTexId;
Shader cubeShader;
Shader skyboxShader;

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

    glm::mat4 view = glm::lookAt(
        cameraPos,
        cameraPos + cameraFront,
        cameraUp
    );
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 1.0f, 100.0f);
    
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

    printf("Moo 2!\n");

    glEnable(GL_DEPTH_TEST);

    // Windows
    //initShaders("src/minimalTex.vert", "src/normalTexture.frag", cubeShaderProgram);
    // OSX

	cubeShader = Shader("src/minimalTex.vert", "src/normalTexture.frag");

    skyboxShader = Shader("src/skybox.vert", "src/skybox.frag");

    // Load vertices and setup attributes
    init();

    // Configure the cube shaders
    glGenTextures(1, &cubeTexId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cubeTexId);

    sf::Image image;

    // Windows
    image.loadFromFile("src/DirtGrass.png");
    // OSX
    //image.loadFromFile("DirtGrass.png");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getSize().x, image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
	cubeShader.setInt("theTexture", 0);

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

    while (!glfwWindowShouldClose(window)) {
		processInput(window);

		//printf("%f %f %f\n", cameraFront.x, cameraFront.y, cameraFront.z);

        auto t_now = chrono::high_resolution_clock::now();
        float time = chrono::duration_cast<chrono::duration<float>>(t_now - t_start).count();

        //model = glm::rotate(model, time * glm::radians(-180.0f), glm::vec3(0.0f, 0.0f, 1.0f));


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

	bool colemak = true;

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
