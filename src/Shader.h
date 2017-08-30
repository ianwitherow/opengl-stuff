#include <stdio.h>

class Shader
{
public:
	unsigned int ID;
	Shader() { }

	Shader(const char* vertFilename, const char* fragFilename) {

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

		ID = glCreateProgram();
		glAttachShader(ID, vertexShader);
		glAttachShader(ID, fragmentShader);

		// Not necessary since 0 is the default, but doing it anyway.
		glBindFragDataLocation(ID, 0, "outColor");

		glLinkProgram(ID);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

	}

	void use() {
		glUseProgram(ID);
	}

	void setInt(const std::string &name, int value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);

	}

	void setFloat(const std::string &name, float value) const {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

	void setMat4(const std::string &name, glm::mat4 value) const {
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
	}

	void Moo() {
		printf("Shader ID: %i\n", ID);
	}

private:

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
		}
		else {
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
		}
		else {
			printf("%s shader failed to load!\n", shaderName);
			char buffer[512];
			glGetShaderInfoLog(shader, 512, NULL, buffer);
			printf("%s", buffer);
		}
	}

};

