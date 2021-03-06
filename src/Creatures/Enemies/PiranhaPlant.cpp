#include "PiranhaPlant.h"
#include "Mario.h"
#include "MarioGame.h"

namespace Creatures
{
	PiranhaPlant::PiranhaPlant()
	{
		sprite.setTexture(*sMarioGame->TextureManager().Get("Enemies"));
		sprite.setTextureRect({ 0, 0, 0, 0 });

		SetSize(size);
	}

	void PiranhaPlant::KickFromTop(Mario* mario)
	{
		if (dead_zone)
			mario->ReciveDamage();
	}

	void PiranhaPlant::KickFromBottom(Mario* mario)
	{
		GetParent()->RemoveObject(this);
		AddScoreToPlayer(800);
		sMarioGame->PlaySound("kick");
	}

	void PiranhaPlant::TouchSide(Mario* mario)
	{
		if (dead_zone)
			mario->ReciveDamage();
	}

	void PiranhaPlant::Fired(Mario* mario)
	{
		GetParent()->RemoveObject(this);
		AddScoreToPlayer(800);
		sMarioGame->PlaySound("kick");
	}

	bool PiranhaPlant::IsAlive() const
	{
		return true;
	}

	void PiranhaPlant::Draw(sf::RenderWindow* render_window)
	{
		sprite.setPosition(GetPosition());
		render_window->draw(sprite);
	}

	void PiranhaPlant::Update(int delta_time)
	{
		Enemy::Update(delta_time);

		int sprite_index = 0;
		float height = 0;
		timer += delta_time;

		dead_zone = (timer > period_time * 0.25 && timer < 3 * period_time);

		if (timer < period_time)
		{
			static const int distance = 20;
			if ((GetMario()->GetBounds().Center() - GetBounds().Center()).Lenght() < 100)
				HideInTube();
			height = 0;
		}
		else if (timer < 1.25 * period_time)
		{
			sprite_index = 1;
			height = ((timer - period_time) / (0.25f * period_time)) * size.Y;
		}
		else if (timer < 3 * period_time)
		{
			sprite_index = int(timer / 100) % 2;
			height = size.Y;
		}
		else if (timer < 3.25 * period_time)
		{
			sprite_index = 0;
			height = (1 - ((timer - 3 * period_time) / (0.25f * period_time))) * size.Y;
		}
		else
			timer = 0;

		SetSize({ size.X, height });
		Move({ 0.f, buttom - GetPosition().Y - height });
		sprite.setTextureRect({ 32 * sprite_index, 80, (int)size.X, (int)height });
	}

	void PiranhaPlant::OnActivated()
	{
		buttom = GetPosition().Y + size.Y;
	}

	void PiranhaPlant::HideInTube()
	{
		timer = 0;
		SetSize({ size.X, 0.f });
		Move({ 0.f, buttom - GetPosition().Y });
	}
}