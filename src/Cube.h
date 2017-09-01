class Cube
{
public:
    glm::vec3 position;
    Shader* shader;
	GLuint textureId;
    static GLuint vao;
	bool markedForGreatness;
    
	Cube() {
		markedForGreatness = false;
	}
    
    Cube(glm::vec3 position, Shader &shader, GLuint textureId)
    : position(position), shader(&shader), textureId(textureId) {
		markedForGreatness = false;
    }
    
    static void Init() {
        printf("Init");
        
        
        // TODO: get these from somewhere else probably
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
        
        
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        
        GLuint cubeVbo;
        glGenBuffers(1, &cubeVbo);
        
        glBindBuffer(GL_ARRAY_BUFFER, cubeVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        
        
    }
    
    static void SetAttributes(Shader shader) {

        GLint posAttrib = glGetAttribLocation(shader.ID, "position");
        glEnableVertexAttribArray(posAttrib);
        __glewVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
        
        GLint colorAttrib = glGetAttribLocation(shader.ID, "color");
        glEnableVertexAttribArray(colorAttrib);
        __glewVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        
        GLint texAttrib = glGetAttribLocation(shader.ID, "texcoord");
        __glewVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(texAttrib);
        
    }
    
    static void bindVao() {
        glBindVertexArray(vao);
    }

    void Render() {
		if (markedForGreatness) {
			printf("Marked for greatness!\n");
		}
		//shader->use();
        glBindVertexArray(vao);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureId);


        glm::mat4 mdl = glm::translate(glm::mat4(1.f), glm::vec3(position.x, position.y, position.z));
        shader->setMat4("model", mdl);

        
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
    }
};

