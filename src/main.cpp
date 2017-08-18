
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

GLuint squareShaderProgram;
GLuint triangleShaderProgram;
GLuint squareVao;
GLuint triangleVao;

GLfloat vertices[] = {
    -0.5f, 0.5f, 0.0f, 0.0f,   // Top-left
    0.5f, 0.5f, 1.0f, 0.0f,    // Top-right
    0.5f, -0.5f, 1.0f, 1.0f,   // Bottom-right
    -0.5f, -0.5f, 0.0f, 1.0f,  // Bottom-left
};


GLfloat triangleVertices[] = {
    0.0f,  0.5f, 1.0f, 0.0f, 0.0f, // Vertex 1 (X, Y)
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Vertex 2 (X, Y)
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f
};

GLint elements[] = {
    0, 1, 2,
    2, 3, 0
};


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

void initShaders(const char* vertFilename, const char* fragFilename, GLuint &shaderProgram) {

    const char* vertexSource = loadFile(vertFilename);
    const char* fragmentSource = loadFile(fragFilename);

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

}

void setupSquareShape() {
    // Create a Vertex Array Object to store the shader program's attribute linking information
	glGenVertexArrays(1, &squareVao);
    glBindVertexArray(squareVao);

    GLuint squareBuffer;
    // Create a buffer, store its id in 'vbo'
    glGenBuffers(1, &squareBuffer);

    // Set that buffer to the 'active' buffer
	glBindBuffer(GL_ARRAY_BUFFER, squareBuffer);

    // Write our vertices to the active buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    GLint posAttrib = glGetAttribLocation(squareShaderProgram, "position");
    // 2, because we're sending x and y for each vertex
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), 0);

    GLint texAttrib = glGetAttribLocation(squareShaderProgram, "texcoord");
    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));

    GLuint texture;
    glGenTextures(1, &texture);

    sf::Image image;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    image.loadFromFile("water.jpg");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getSize().x, image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
    glUniform1i(glGetUniformLocation(squareShaderProgram, "waterTexture"), 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
}


void setupTriangleShape() {
    printf("Shader program: %i\n", triangleShaderProgram);
    
	// Load triangle data
	glGenVertexArrays(1, &triangleVao);
    glBindVertexArray(triangleVao);

    GLuint triangleBuffer;
    glGenBuffers(1, &triangleBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, triangleBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);


    GLint posAttrib = glGetAttribLocation(triangleShaderProgram, "position");
    // 2, because we're sending x and y for each vertex
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);


    GLint colorAttrib = glGetAttribLocation(triangleShaderProgram, "color");
    glEnableVertexAttribArray(colorAttrib);
    glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(2*sizeof(float)));
}


void renderBg() {
	glClearColor(0.2f, 0.3f, 0.4f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void renderSquare() {
	//use square shader program
	glUseProgram(squareShaderProgram);
    glBindVertexArray(squareVao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}

void renderTriangle() {
	glUseProgram(triangleShaderProgram);
    glBindVertexArray(triangleVao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void reshape(int w, int h)
{
    // keep aspect ratio
    h = w / 1.333333;
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
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

    initShaders("minimalTex.vert", "water.frag", squareShaderProgram);
	setupSquareShape();

	initShaders("minimal.vert", "color.frag", triangleShaderProgram);
	setupTriangleShape();
    
    GLint e = glGetError();
    printf("%i\n", e);
    
    

    auto start = std::chrono::high_resolution_clock::now();
    GLuint sqTimeUniform = glGetUniformLocation(squareShaderProgram, "time");
    GLuint triTimeUniform = glGetUniformLocation(triangleShaderProgram, "time");

    
    bool running = true;

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
        auto now = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration_cast<std::chrono::duration<float>>(now - start).count();
        
		glUseProgram(squareShaderProgram);
        glUniform1f(sqTimeUniform, (sin(time * 4.0f) + 1.0f) / 2.0f);
		glUseProgram(triangleShaderProgram);
        glUniform1f(triTimeUniform, (sin(time * 4.0f) + 1.0f) / 2.0f);
        
        // Draw objects
        renderBg();
        renderSquare();
        renderTriangle();

        // Switch to the front buffer
        window.display();

    }

    return 0;
}
