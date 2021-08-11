#pragma once

#include "enums.hpp"

// std
#include <vector>

namespace vmc {

	const int MAX_CHUNK_HEIGHT = 16;

	class Chunk
	{
	public:
		Chunk(int width, int height = MAX_CHUNK_HEIGHT);
		std::vector<BlockFace> getVisibleBlockFaces(int x, int y, int z);

		void visibleBlockFacesTest();
	private:
		int height;
		int width;
		std::vector<std::vector<std::vector<BlockType>>>blockMap;

	};
}

