#ifndef BRICKS_BLOCK_H
#define BRICKS_BLOCK_H

#include "Block.h"

namespace GameObjects
{
	class BricksBlock : public Block
	{
	public:
		BricksBlock();

		virtual void Draw(sf::RenderWindow* render_window);
		virtual void Kick(Creatures::Mario* mario) override;
		virtual void Update(int delta_time);

	private:
		SpriteSheet sprite_sheet;
		float kicked_value = 0;
		int kicked_dir = 0;
	};
}

#endif // !BRICKS_BLOCK_H