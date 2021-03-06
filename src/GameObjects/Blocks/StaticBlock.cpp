#include "StaticBlock.h"
#include "MarioGame.h"

namespace GameObjects
{
	SpriteSheet StaticBlock::sprite_sheet = SpriteSheet();

	StaticBlock::StaticBlock(BlockTitleCode code) : Block(code)
	{
		if (sprite_sheet.Empty())
			sprite_sheet.Load(*sMarioGame->TextureManager().Get("Tiles"), Vector(0, 0), { 32, 32 }, 8, 12);
		current_sprite = &sprite_sheet[id - 1];

		colliable = id <= 40;

		if (id == 14 || id == 22)
			colliable = false;
	}

	void StaticBlock::Draw(sf::RenderWindow* render_window)
	{
		current_sprite->setPosition(position * block_size);
		render_window->draw(*current_sprite);
	}

	void StaticBlock::Kick(Creatures::Mario* mario)
	{
		sMarioGame->PlaySound("bump");
	}
}