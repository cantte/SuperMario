#include "TwistedCoin.h"
#include "MarioGame.h"

using namespace GameEngine;

namespace GameObjects
{
	TwistedCoin::TwistedCoin(const Vector& pos, Creatures::Mario* mario)
	{
		SetPosition(pos);
		speed = Vector::Up * 0.05f;
		speed.Y = -0.20f;

		animator.Create("twist", *sMarioGame->TextureManager().Get("Items"), Vector(0, 84), Vector(32, 32), 4, 1, 0.01f);
		animator.Create("shine", *sMarioGame->TextureManager().Get("Items"), Vector(0, 116), Vector(40, 32), 5, 1, 0.01f, AnimType::Forward);
		animator.Get("shine")->SetOrigin(Vector(4, 0));
	}

	void TwistedCoin::Draw(sf::RenderWindow* render_window)
	{
		animator.SetPosition(GetPosition());
		animator.Draw(render_window);
	}

	void TwistedCoin::Update(int delta_time)
	{
		if (timer == 0)
		{
			sMarioGame->PlaySound("coin");
		}
		else if (timer < 700)
		{
			speed.Y += delta_time * 0.0005f;
			Move(speed * delta_time);
		}
		else if (timer < 1200)
		{
			animator.Play("shine");
		}
		else
		{
			GetParent()->RemoveObject(this);
		}

		timer += delta_time;
		animator.Update(delta_time);
	}
}