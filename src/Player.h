class Player
{
public:
    Camera camera;
    float height = 2.5f;
    bool jumping = false;
    bool falling = false;
    float fallStart = 0.f;
    float yVelocity = 0.f;
    bool grounded = true;
    int jumpIndex = 0;
    vector<float> yCoords;
    
    Player() {
    }
    
    void fallTo(float y) {
        falling = true;
        float startingY = camera.position.y;
        float distanceToFall = camera.position.y - y;
        printf("Distance to fall: %f\n", distanceToFall);
        float gravity = 1;
        float v0 = sqrt(0.667f * startingY * gravity);
        float upperBound = 2 * (v0 / gravity);

        for (float t = 0.f; t <= upperBound; t += (upperBound / 30)) {
            //float yValue = startingY - 0.5 * gravity * t * t;
            float yValue = startingY - (v0 * t) - 0.5 * gravity * t * t;
            //printf("%f\n", yValue);
            yCoords.push_back(yValue);
        }

        // End up where we want
        yCoords.push_back(y);
        
    }
    
    void Jump() {
        if (!grounded || jumping) {
            // Can't jump while jumping :p
            printf("Can't jump\n");
            return;
        }
        jumping = true;
        
        yCoords.clear();
        //float vNought = (float)10 / (float)6;
        //float gravity = 1;
        //float gravity = (float)1 / (float)30;
        
        
        float startingY = camera.position.y + .001f;
        float jumpPeak = height + 2.5f;
        float gravity = 1;
        float v0 = sqrt(0.667f * jumpPeak * gravity);
        float upperBound = 2 * (v0 / gravity);
        
        for (float t = 0.f; t <= upperBound; t += (upperBound / 30)) {
            //float yValue = (startingY - (v0 * t) + (gravity * (t * t)));
            float yValue = startingY + (v0 * t) - 0.5 * gravity * t * t;
            //printf("%f\n", yValue);
            yCoords.push_back(yValue);
        }
        
        // Back to normal when we're done
        yCoords.push_back(startingY);
        
        //printf("%lu\n", yCoords.size());
        
    }
    
};
