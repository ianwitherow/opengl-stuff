
class Camera
{
public:

    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;

    float speed = 0.1f;
    float Yaw = -180.0f;
    float Pitch = 0.0f;
    float mouseSensitivity = 0.1f;

    // Mouse stuff
    float lastX, lastY;
    bool firstMouse = false;

    Camera() {}
    Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up) : position(position), front(front), up(up) {
    }

    glm::mat4 getView() {

		glm::mat4 lookAt = glm::lookAt(
             position,
             position + front,
             up
         );


		return lookAt;
    }

    void moveForward(bool freeFly) {
        if (!freeFly) {
            printf("Not in free fly\n");
            // If not in freefly, don't allow moving up or down
            this->position += speed * glm::vec3(front.x, 0.f, front.z);
        } else {
            printf("In free fly\n");
            this->position += speed * front;
        }
		printf("moveForward: %f %f %f\n", position.x, position.y, position.z);
    }
    
    void handleMouseMove(double xpos, double ypos) {
        if (firstMouse) {
            lastX = (float)xpos;
            lastY = (float)ypos;
            firstMouse = false;
        }

        float xoffset = (float)xpos - lastX;
        float yoffset = (float)ypos - lastY;

        lastX = (float)xpos;
        lastY = (float)ypos;

        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;
        yoffset = -yoffset;

        Yaw += xoffset;
        Pitch += yoffset;

        front = glm::normalize(glm::vec3(
            cos(glm::radians(Yaw)) * cos(glm::radians(Pitch)),
            sin(glm::radians(Pitch)),
            sin(glm::radians(Yaw)) * cos(glm::radians(Pitch))
        ));

    }

};
