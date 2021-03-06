#include "Ladder.h"
#include "Mario.h"
#include "MarioGame.h"

using namespace Creatures;

namespace GameObjects
{
	Ladder::Ladder(const Vector& pos, Mario* mario)
	{
		SetBounds({ pos.X + 10.f, 0.f, 10.f, pos.Y + 32.f });
		sprite.setTexture(*sMarioGame->TextureManager().Get("Items"));
	}

	void Ladder::Draw(sf::RenderWindow* render_window)
	{
		int k = GetBounds().Height / 32;
		int pos_x = GetBounds().Center().X - 32 / 2;

		for (int i = 0; i <= k; ++i)
		{
			sprite.setTextureRect({ 212, !i ? 0 : (64 - (i % 2) * 32), 32, i == k ? (int(GetBounds().Height) % 32) : 32 });
			sprite.setPosition(pos_x, i * 32 + GetPosition().Y);
			render_window->draw(sprite);
		}
	}

	void Ladder::Update(int delta_time)
	{
		GameObject::Update(delta_time);

		if (timer == 0)
			sMarioGame->PlaySound("powerup_apperars");
		timer += delta_time;

		float current_height = timer * 0.04f;
		if (current_height < height)
			SetBounds({ GetPosition().X, bottom - current_height,  width, current_height });
	}

	void Ladder::OnActivated()
	{
		height = GetBounds().Height;
		width = GetBounds().Width;
		bottom = GetPosition().Y + height;
	}

	void Ladder::Init()
	{
		GetParent()->FindObjectByName<Mario>("Mario")->MoveToFront();
	}
}