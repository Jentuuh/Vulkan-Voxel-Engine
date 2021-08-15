#include "chunk_component.hpp"
#include "vmc_model.hpp"
#include "block_model.hpp"

#include <iostream>
namespace vmc {
	ChunkComponent::ChunkComponent(int width, int height) : width{ width }, height{ height } {
		blockMap.resize(height);


		for (int i = 0; i < height; i++) {
			blockMap[i].resize(width);
			for (int j = 0; j < width; j++) {
				blockMap[i][j].resize(width);
				for (int k = 0; k < width; k++) {
					// Initialize a chunk with dirt blocks, except for the second layer, which is air
					i == 1 ? blockMap[i][j][k] = BlockType::air : blockMap[i][j][k] = BlockType::dirt;
				}
			}
		}
	}


	std::vector<BlockFace> ChunkComponent::getVisibleBlockFaces(int x, int y, int z) const
	{
		std::vector<BlockFace> visibleFaces(0);

		// Don't draw any 'air' blocks
		if (blockMap[y][z][x] == BlockType::air) return visibleFaces;
		
		// Down check
		if (y + 1 >= blockMap.size()) {
			visibleFaces.push_back(BlockFace::down);
		}
		else if (blockMap[y + 1][z][x] == BlockType::air) {
			visibleFaces.push_back(BlockFace::down);
		}

		// Up check
		if (y - 1 < 0) {
			visibleFaces.push_back(BlockFace::up);
		}
		else if (blockMap[y - 1][z][x] == BlockType::air) {
			visibleFaces.push_back(BlockFace::up);
		}

		// Back check
		if (z + 1 >= blockMap[0].size()) {
			visibleFaces.push_back(BlockFace::back);
		}
		else if (blockMap[y][z + 1][x] == BlockType::air) {
			visibleFaces.push_back(BlockFace::back);
		}

		// Front check
		if (z - 1 < 0) {
			visibleFaces.push_back(BlockFace::front);
		}
		else if (blockMap[y][z - 1][x] == BlockType::air) {
			visibleFaces.push_back(BlockFace::front);
		}

		// Right check
		if (x + 1 >= blockMap[0][0].size()) {
			visibleFaces.push_back(BlockFace::right);
		}
		else if (blockMap[y][z][x + 1] == BlockType::air) {
			visibleFaces.push_back(BlockFace::right);
		}

		// Left check
		if (x - 1 < 0) {
			visibleFaces.push_back(BlockFace::left);
		}
		else if (blockMap[y][z][x - 1] == BlockType::air) {
			visibleFaces.push_back(BlockFace::left);
		}

		return visibleFaces;
	}

	void ChunkComponent::visibleBlockFacesTest() {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				for (int z = 0; z < width; z++) {
					std::vector<BlockFace> visibleFaces = getVisibleBlockFaces(z, i, j);

					for (BlockFace face : visibleFaces) {
						switch (face)
						{
						case vmc::BlockFace::up:
							std::cout << " U ";
							break;
						case vmc::BlockFace::down:
							std::cout << " D ";
							break;
						case vmc::BlockFace::left:
							std::cout << " L ";
							break;
						case vmc::BlockFace::right:
							std::cout << " R ";
							break;
						case vmc::BlockFace::front:
							std::cout << " F ";
							break;
						case vmc::BlockFace::back:
							std::cout << " B ";
							break;
						default:
							break;
						}
					}
					std::cout << " || ";
				}
				std::cout << "\n";
			}
			std::cout << "---------------------------------------------------------------------------\n";
		}
	}
}