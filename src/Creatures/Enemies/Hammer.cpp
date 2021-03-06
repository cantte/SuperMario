#include "Hammer.h"
#include "Mario.h"
#include "MarioGame.h"

using namespace GameEngine;

namespace Creatures
{
	Hammer::Hammer(Mario* target) : target(target)
	{
		SetSize({ 32, 32 });

		const sf::Texture& texture = *sMarioGame->TextureManager().Get("Enemies");
		animator.Create("fly", texture, Vector(96, 112), Vector(32, 32), 4, 1, 0.01f);
		animator.Create("in_hand", texture, { 96,112,32,32 });
		animator.Play("in_hand");
	}

	void Hammer::Draw(sf::RenderWindow* render_window)
	{
		animator.SetPosition(GetPosition());
		animator.Draw(render_window);
	}

	void Hammer::Update(int delta_time)
	{
		if (state == State::Fly)
		{
			animator.Update(delta_time);
			speed += Vector::Down * gravity_force * delta_time;
			Move(speed * delta_time);

			if (GetPosition().Y > 1000)
				GetParent()->RemoveObject(this);
			if (target && target->GetBounds().IsContain(GetBounds().Center()))
				target->ReciveDamage();
		}
	}

	void Hammer::ThrowAway(const Vector& speed)
	{
		this->speed = speed;
		animator.Play("fly");
		state = State::Fly;
	}
}