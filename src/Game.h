
class Game
{
public:
    Player player;
    
    // State
    bool paused = false;
    bool freeFly = false;

    GLFWwindow* window;
    
	World world;

    double pausedMouseX;
    double pausedMouseY;
    
    unsigned int screenWidth;
    unsigned int screenHeight;
    
    Game(unsigned int screenWidth, unsigned int screenHeight)
        : screenWidth(screenWidth), screenHeight(screenHeight) {
		world = World();
    }

    int createWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
        
        this->window = glfwCreateWindow(this->screenWidth, this->screenHeight, "Moo!", NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(window);
        // vsync on
        glfwSwapInterval(1);

        return 1;
    };

    void pause() {
        paused = true;
        printf("Paused\n");
        // Save mouse pos
        glfwGetCursorPos(window, &pausedMouseX, &pausedMouseY);
        
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        
    }
    
    void resume() {
        paused = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPos(window, pausedMouseX, pausedMouseY);
        player.camera.lastX = (float)pausedMouseX;
        player.camera.lastY = (float)pausedMouseY;
        
    }

	void tick() {

	}
    
};
