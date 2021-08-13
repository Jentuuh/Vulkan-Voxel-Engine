#pragma once

#include "enums.hpp"

// std
#include <vector>

namespace vmc {

	const int MAX_CHUNK_HEIGHT = 16;
	const int BLOCK_X_OFFSET = 1;
	const int BLOCK_Y_OFFSET = 1;
	const int BLOCK_Z_OFFSET = 1;

	class ChunkComponent
	{
	public:
		ChunkComponent(int width, int height = MAX_CHUNK_HEIGHT);

		int getWidth() const{ return width; }
		int getHeight() const{ return height; }
	
		std::vector<BlockFace> getVisibleBlockFaces(int x, int y, int z) const;
		void visibleBlockFacesTest();
	private:
		

		int height;
		int width;
		std::vector<std::vector<std::vector<BlockType>>>blockMap;
	};
}

