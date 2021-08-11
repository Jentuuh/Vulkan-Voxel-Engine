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

	private:
		std::vector<std::vector<std::vector<BlockType>>>blockMap;

	};
}

