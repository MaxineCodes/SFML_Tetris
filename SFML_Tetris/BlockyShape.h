#pragma once
#include <SFML/Graphics.hpp>

enum class BlockType
{
	square,
	line,
	tshape,
	lshape,
	zshape,
	count
};

class BlockyShape
{
	public: 
		BlockyShape(BlockType type);
		sf::Color getColour();
		BlockyShape getType();
		sf::Vector2i mBlocks[4];
	private:
		sf::Color mColour;
};