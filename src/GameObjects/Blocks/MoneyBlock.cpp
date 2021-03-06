#include "MoneyBlock.h"
#include "TwistedCoin.h"
#include "MarioGame.h"
#include "Blocks.h"

namespace GameObjects
{
	MoneyBlock::MoneyBlock() : Block(Money)
	{
		sprite_sheet.Load(*sMarioGame->TextureManager().Get("Tiles"), { { 32,0,32,32 }, { 0,32,32,32 } });
	}

	void MoneyBlock::Draw(sf::RenderWindow* render_window)
	{
		sprite_sheet.SetPosition(position * block_size + Vector::Up * kicked_value);
		sprite_sheet.Draw(render_window);
	}

	void MoneyBlock::Kick(Creatures::Mario* mario)
	{
		if (kicked_value == 0 && coin_left != 0)
		{
			kicked_value = 1;
			kicked_dir = 0;

			Vector pos = (position + Vector::Up) * block_size;
			blocks->AddObject(new TwistedCoin(pos, mario));

			--coin_left;
			if (coin_left == 0)
				sprite_sheet.SetSpriteIndex(1);

			sMarioGame->AddScore(100, blocks->ToPixelCoordinates(position));
			sMarioGame->AddCoin();
			sMarioGame->PlaySound("bump");
		}
	}

	void MoneyBlock::Update(int delta_time)
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
