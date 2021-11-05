#include "BlockyShape.h"

BlockyShape::BlockyShape(BlockType type)
{
	switch (type)
	{
	case BlockType::square:
		mBlocks[0] = sf::Vector2i(0, 0);
		mBlocks[1] = sf::Vector2i(1, 0);
		mBlocks[2] = sf::Vector2i(0, 1);
		mBlocks[3] = sf::Vector2i(1, 1);

		mColour = sf::Color::Blue;	// temporary for debugging
		break;
	case BlockType::line:
		mBlocks[0] = sf::Vector2i(0, 0);
		mBlocks[1] = sf::Vector2i(0, 1);
		mBlocks[2] = sf::Vector2i(1, 1);
		mBlocks[3] = sf::Vector2i(1, 1);

		mColour = sf::Color::Green;	// temporary for debugging
		break;
	case BlockType::tshape:
		mBlocks[0] = sf::Vector2i(0, 0);
		mBlocks[1] = sf::Vector2i(0, 1);
		mBlocks[2] = sf::Vector2i(0, 2);
		mBlocks[3] = sf::Vector2i(1, 2);

		mColour = sf::Color::Red;	// temporary for debugging
		break;
	case BlockType::lshape:
		mBlocks[0] = sf::Vector2i(0, 0);
		mBlocks[1] = sf::Vector2i(-1, 0);
		mBlocks[2] = sf::Vector2i(0, 1);
		mBlocks[3] = sf::Vector2i(1, 0);

		mColour = sf::Color::Yellow;	// temporary for debugging
		break;
	case BlockType::zshape:
		mBlocks[0] = sf::Vector2i(0, 0);
		mBlocks[1] = sf::Vector2i(0, 1);
		mBlocks[2] = sf::Vector2i(0, 2);
		mBlocks[3] = sf::Vector2i(0, 3);

		mColour = sf::Color::Cyan;	// temporary for debugging
		break;
	default:
		mColour = sf::Color::Black;	// temporary for debugging
		break;
	}
}

sf::Color BlockyShape::getColour() 
{
	return mColour;
}