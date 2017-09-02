
class World
{
public:
	mutable vector<Cube> blocks;
    
	World() {


	}

	void Render() {
		unsigned int lastShaderId = 0;
        for (std::vector<Cube>::iterator it = blocks.begin(); it != blocks.end(); ++it) {
            Cube & block = *it;
			if (block.shader->ID != lastShaderId) {
				block.shader->use();
				lastShaderId = block.shader->ID;
			}
			block.Render();
		}
	}

};
