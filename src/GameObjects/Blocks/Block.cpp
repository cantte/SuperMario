#include "Block.h"
#include "Blocks.h"
#include "Enemy.h"
#include "Mario.h"
#include "Mushroom.h"

using namespace Creatures;

namespace GameObjects
{
	SpriteSheet Block::question_block_sprite = SpriteSheet();
	SpriteSheet Block::water_sprite = SpriteSheet();
	SpriteSheet Block::lava_sprite = SpriteSheet();
	SpriteSheet Block::kicked_sprite = SpriteSheet();
	SpriteSheet Block::bricket_sprite = SpriteSheet();
	int Block::block_size = 0;
	float Block::timer = 0;

	Block::Block(BlockTitleCode id)
	{
		this->id = id;
	}

	void Block::SetInvisible(bool value)
	{
		invisible = value;
		colliable = !value;
		current_sprite = &kicked_sprite;
	}

	void Block::KillEnemiesAbove(Mario* mario)
	{
		Rect block_rect = blocks->GetBlockBounds(position.X, position.Y - 1).Bordered(-4);

		auto func = [block_rect, mario](GameObject* obj)
		{
			if (obj->GetBounds().IsIntersect(block_rect))
			{
				if (obj->IsTypeOf<Enemy>())
				{
					auto enemy = obj->CastTo<Enemy>();

					if (enemy->IsAlive())
						enemy->KickFromBottom(mario);
				}
				else if (obj->IsTypeOf<Mushroom>())
				{

				}
			}
		};
	}
}