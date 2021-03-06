#ifndef MONEY_BLOCK_H
#define MONEY_BLOCK_H

#include "Block.h"

namespace GameObjects
{
	class MoneyBlock : public Block
	{
	public:
		MoneyBlock();

		virtual void Draw(sf::RenderWindow* render_window);
		virtual void Kick(Creatures::Mario* mario) override;
		virtual void Update(int delta_time);

	private:
		SpriteSheet sprite_sheet;
		int coin_left = 5;
		float kicked_value = 0;
		int kicked_dir = 0;
	};
}

#endif // !MONEY_BLOCK_H
