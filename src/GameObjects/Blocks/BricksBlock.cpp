#include "BricksBlock.h"
#include "MarioGame.h"
#include "Mario.h"
#include "Blocks.h"
#include "OneBrick.h"

namespace GameObjects
{
	BricksBlock::BricksBlock() : Block(Brick)
	{
		sprite_sheet.Load(*sMarioGame->TextureManager().Get("Tiles"), { { 32,0,32,32 } });
	}

	void BricksBlock::Draw(sf::RenderWindow* render_window)
	{
		sprite_sheet.SetPosition(position * block_size + Vector::Up * kicked_value);
		sprite_sheet.Draw(render_window);
	}

	void BricksBlock::Kick(Creatures::Mario* mario)
	{
		int x = position.X;
		int y = position.Y;

		if (!mario->IsSmall())
		{
			blocks->AddObject(new OneBrick(Vector(x, y) * block_size, Vector(-0.05f, -0.2f)));
			blocks->AddObject(new OneBrick(Vector(x + 0.5f, (float)y) * block_size, Vector(0.05f, -0.2f)));
			blocks->AddObject(new OneBrick(Vector((float)x, y + 0.5f) * block_size, Vector(-0.05f, -0.1f)));
			blocks->AddObject(new OneBrick(Vector(x + 0.5f, y + 0.5f) * block_size, Vector(0.05f, -0.1f)));

			KillEnemiesAbove(mario);
			blocks->ClearBlock(x, y);

			sMarioGame->AddScore(50);
			sMarioGame->PlaySound("breakblock");
		}
		else
		{
			if (kicked_value == 0)
			{
				KillEnemiesAbove(mario);
				kicked_value = 1;
				kicked_dir = 0;
				sMarioGame->PlaySound("bump");
			}
		}
	}

	void BricksBlock::Update(int delta_time)
	{
		if (kicked_value != 0)
		{
			if (kicked_dir == 0)
			{
				kicked_value += delta_time / 10;

				if (kicked_value > 15)
					kicked_dir = 1;
			}
			if (kicked_dir == 1)
			{
				kicked_value -= delta_time / 10;
				if (kicked_value < 0)
					kicked_value = 0;
			}
		}
	}
}