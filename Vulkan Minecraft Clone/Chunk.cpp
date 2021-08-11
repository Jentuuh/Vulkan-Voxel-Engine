#include "Chunk.hpp"

#include <iostream>
namespace vmc {
	Chunk::Chunk(int width, int height) {
		blockMap.resize(height);
		for (std::vector col : blockMap) {
			col.resize(width);
			std::cout << "------------------------------------------------------------" << '\n';
			for (std::vector row : col) {
				std::cout << '\n';
				row.resize(width);
				for (int i = 0; i < width; i++) {
					// Initialize a chunk with air
					row[i] = BlockType::air;
				}
				for (BlockType block : row) {
					std::cout << "X" << " || ";
				}
			}
			std::cout << '\n';
		}
	}
}