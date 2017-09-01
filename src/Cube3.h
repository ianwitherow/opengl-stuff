#pragma once

class Cube3
{
public:
	glm::vec3 position;
	Shader* shader;
	static GLuint vao;

	Cube3(glm::vec3 position, Shader &shader);

	static void Init();
	static void SetAttributes(Shader shader);
	void Render(GLuint textureId = -1);
};

