#ifndef STATIC_BLOCK_H
#define STATIC_BLOCK_H

#include "Block.h"

namespace GameObjects
{
	class StaticBlock : public Block
	{
	public:
		StaticBlock(BlockTitleCode id);

		virtual void Draw(sf::RenderWindow* render_window) override;
		virtual void Kick(Creatures::Mario* mario) override;

	private:
		static SpriteSheet sprite_sheet;
		sf::Sprite* current_sprite;

		friend class Blocks;
	};
}

#endif // !STATIC_BLOCK_H
