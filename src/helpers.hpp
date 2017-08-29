//
//  helpers.hpp
//  Moo
//
//  Created by Ian Witherow on 8/23/17.
//  Copyright © 2017 Ian Witherow. All rights reserved.
//

#ifndef helpers_hpp
#define helpers_hpp


using namespace std;

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

#endif /* helpers_hpp */
