
#include <stdlib.h>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <GL/glew.h>
#include <GLUT/GLUT.h>
#include <OpenGL/gl.h>

#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

using namespace std;

GLfloat vertices[] = {
    0.0f, 0.5f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
    -1.0f, 0.5f, 0.0f, 1.0f, 0.0f,
};

GLint elements[] = { 0, 1, 2, 2, 3, 0 };

char* loadFile(const char *fname) {
    ifstream::pos_type size;
    char * memblock;

    ifstream file(fname, ios::in | ios::binary | ios::ate);
    if (file.is_open()) {
        size = file.tellg();
        memblock = new char[(int)size + 1];
        file.seekg(0, ios::beg);
        file.read(memblock, size);
        file.close();
        memblock[size] = '\0';
        cout << "file " << fname << " loaded" << endl;
    } else {
        cout << "Unable to open file " << fname << endl;
        exit(1);
    }
    return memblock;
}

void init() {
    GLuint vbo;
    // Create a buffer, store its id in 'vbo'
    glGenBuffers(1, &vbo);

    // Set that buffer to the 'active' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Write our vertices to the active buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Create a Vertex Array Object to store the shader program's attribute linking information
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

}

void printShaderCompileResult(GLuint shader, const char* shaderName) {
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_TRUE) {
        printf("%s shader loaded!\n", shaderName);
    } else {
        printf("%s shader failed to load!\n", shaderName);
        char buffer[512];
        glGetShaderInfoLog(shader, 512, NULL, buffer);
        printf("%s", buffer);
    }

}

void initShaders(GLuint &shaderProgram) {

    const char* vertexSource = loadFile("minimal.vert");
    const char* fragmentSource = loadFile("minimal.frag");

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    printShaderCompileResult(vertexShader, "Vertex");

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);
    printShaderCompileResult(fragmentShader, "Fragment");

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    // Not necessary since 0 is the default, but doing it anyway.
    glBindFragDataLocation(shaderProgram, 0, "outColor");

    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    // 2, because we're sending x and y for each vertex
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0);
    glEnableVertexAttribArray(posAttrib);

    GLint colorAttrib = glGetAttribLocation(shaderProgram, "color");
    // 3, because we're sending r g b
    glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(colorAttrib);
    
}

void render() {
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void reshape(int w, int h)
{
    // keep aspect ratio
    h = w / 1.333333;
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

int main(int argc, char *argv[]) {
    printf("Moo!");
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 2; // Optional
    // Request OpenGL version 3.2 (optional but recommended)
    settings.majorVersion = 3;
    settings.minorVersion = 2;
    settings.attributeFlags = sf::ContextSettings::Core;

    sf::Window window(sf::VideoMode(2200, 1600), "Moo!", sf::Style::Close, settings);

    window.setFramerateLimit(60);
    
    glewExperimental = GL_TRUE;
    glewInit();

    init();

    GLuint shaderProgram;
    initShaders(shaderProgram);

    //GLint uniColor = glGetUniformLocation(shaderProgram, "triangleColor");

    bool running = true;
    //auto t_start = std::chrono::high_resolution_clock::now();

    while (running) {
        sf::Event windowEvent;
        while (window.pollEvent(windowEvent)) {
            switch (windowEvent.type) {
                case sf::Event::Closed:
                    running = false;
                    break;
                case sf::Event::KeyPressed:
                    if (windowEvent.key.code == sf::Keyboard::Escape) {
                        running = false;
                    }
                    break;
                default:
                    break;
            }
        }

        // Draw objects
        render();

        // Switch to the front buffer
        window.display();

    }

    return 0;
}
