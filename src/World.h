
class World
{
public:
	mutable vector<Cube> blocks;
    
	World() {


	}

	void Render() {
		unsigned int lastShaderId = 0;
		for each (Cube block in blocks) {
			if (block.shader->ID != lastShaderId) {
				block.shader->use();
				lastShaderId = block.shader->ID;
			}
			block.Render();
		}
	}

};
