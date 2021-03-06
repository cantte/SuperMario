#include "Blooper.h"
#include "Mario.h"
#include "MarioGame.h"

namespace Creatures
{
	Blooper::Blooper()
	{
		SetSize({ 32, 48 });

		const sf::Texture& texture = *sMarioGame->TextureManager().Get("Enemies");
		animator.Create("zig", texture, { 224, 161, 31, 48 });
		animator.Create("zag", texture, { 256,161,32,48 });
		animator.Create("died", texture, { 224,161 + 48,32,-48 });

	}

	void Blooper::Draw(sf::RenderWindow* render_window)
	{
		animator.SetPosition(GetPosition());
		animator.Draw(render_window);
	}

	void Blooper::Update(int delta_time)
	{
		Enemy::Update(delta_time);

		if (std::abs(GetMario()->GetPosition().X - GetPosition().X) < sMarioGame->ScreenSize().X / 2)
		{
			if (state == State::Zig || state == State::Zag)
			{
				delay_time -= delta_time;
				if (delay_time < 0)
					SetState((state == State::Zag) ? State::Zig : State::Zag);
			}

			Move(delta_time * speed);
			animator.Update(delta_time);
		}
	}

	void Blooper::KickFromTop(Mario* mario)
	{
		mario->ReciveDamage();
	}

	void Blooper::KickFromBottom(Mario* mario)
	{
		SetState(State::Died);
	}

	void Blooper::TouchSide(Mario* mario)
	{
		mario->ReciveDamage();
	}

	bool Blooper::IsAlive() const
	{
		return state != State::Died;
	}

	void Blooper::SetState(State state)
	{
		this->state = state;

		switch (this->state)
		{
			case State::Zig:
			{
				speed = -Vector(1, 1) * 0.15f;
				animator.Play("zig");
				delay_time = 400;

				break;
			}
			case State::Zag:
			{
				speed = Vector::Down * 0.05f;
				animator.Play("zag");
				delay_time = 1200;

				break;
			}
			case State::Died:
			{
				animator.Play("died");
				speed = Vector::Down * 0.2f;

				break;
			}
		}
	}
}