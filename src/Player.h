
class Player
{
public:
    Camera camera;
	float height = 2.5f;
	bool jumping = false;
	bool grounded = true;
	int jumpIndex = 0;
	vector<float> yCoords;

    Player() { }

	void Jump() {
		if (!grounded || jumping) {
			// Can't jump while jumping :p
			return;
		}
		jumping = true;

		yCoords.clear();
		//float vNought = (float)10 / (float)6;
		//float gravity = 1;
		//float gravity = (float)1 / (float)30;


		float startingY = camera.position.y;
		float jumpPeak = height + 1.5f;
		float gravity = 10;
		float v0 = sqrt(0.667f * jumpPeak * gravity);
		float upperBound = 2 * (v0 / gravity);

		for (float t = 0.f; t <= upperBound; t += (upperBound / 60)) {
			//float yValue = (startingY - (v0 * t) + (gravity * (t * t)));
			float yValue = startingY + (v0 * t) - 0.5 * gravity * t * t;
			printf("%f\n", yValue);
			if (yValue > height) {
				yCoords.push_back(yValue);
			}
		}

		// Back to normal when we're done
		yCoords.push_back(startingY);

		printf("%i", yCoords.size());

	}

};
