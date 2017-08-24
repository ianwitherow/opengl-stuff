//#include "moo_includes.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <GL/glew.h>
#include <GLUT/GLUT.h>
#include <OpenGL/gl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <math.h>

#include "helpers.hpp"


using namespace std;

GLfloat vertices[] {
    1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 1.0f
};



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
	glClearColor(0.2f, 0.3f, 0.4f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void render() {
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main(int argc, char *argv[]) {
    printf("Moo!\n");
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 2; // Optional
    // Request OpenGL version 3.2 (optional but recommended)
    settings.majorVersion = 3;
    settings.minorVersion = 2;
    settings.attributeFlags = sf::ContextSettings::Core;

    sf::Window window(sf::VideoMode(2000, 1500), "Moo!", sf::Style::Close, settings);

    window.setFramerateLimit(60);

    glewExperimental = GL_TRUE;
    glewInit();
    
    init();

    GLuint shaderProgram;
    initShaders("minimal.vert", "color.frag", shaderProgram);

    glUseProgram(shaderProgram);
    
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    __glewVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0);
    
    GLint colorAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colorAttrib);
    __glewVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(2*sizeof(float)));

    bool running = true;

    GLint e = glGetError();

    printf("%i\r", e);

    GLint uniTrans = glGetUniformLocation(shaderProgram, "trans");
    auto t_start = chrono::high_resolution_clock::now();

    while (running) {
        sf::Event windowEvent;
        while (window.pollEvent(windowEvent)) {
            switch (windowEvent.type) {
                case sf::Event::Closed:
                    running = false;
                    break;
                case sf::Event::KeyPressed:
                    switch (windowEvent.key.code) {
                        case sf::Keyboard::Escape:
                            running = false;
                            break;
                        case sf::Keyboard::Right:
                            printf("Right\n");
                            break;
                        case sf::Keyboard::Left:
                            printf("Left\n");
                            break;
                        case sf::Keyboard::Up:
                            printf("Up\n");
                            break;
                        case sf::Keyboard::Down:
                            printf("Down\n");
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }

        auto t_now = chrono::high_resolution_clock::now();
        float time = chrono::duration_cast<chrono::duration<float>>(t_now - t_start).count();

        
        glm::mat4 trans;
        trans = glm::rotate(trans, time * glm::radians(-180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

        glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(trans));

        
        // Draw objects
        renderBg();
        render();

        // Switch to the front buffer
        window.display();

    }

    return 0;
}
