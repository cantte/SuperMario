#include "Spinny.h"
#include "MarioGame.h"
#include "Mario.h"

namespace Creatures
{
	Spinny::Spinny(const Vector& position, const Vector& speed, const Vector& walk_direction)
	{
		SetPosition(position);
		this->speed = speed;
		this->walk_direction = walk_direction;
		SetSize({ 32, 32 });

		const sf::Texture& texture = *sMarioGame->TextureManager().Get("Enemies");
		animator.Create("walk", texture, Vector(64, 80), Vector(32, 32), 2, 1, 0.005f);
		animator.Create("egg", texture, Vector(128, 80), Vector(32, 32), 2, 1, 0.005f);
		animator.Create("died", texture, { 64,80 + 32,32,-32 });

		SetState(State::Egg);
	}

	void Spinny::Draw(sf::RenderWindow* render_window)
	{
		animator.SetPosition(GetPosition());
		animator.Draw(render_window);
	}

	void Spinny::Update(int delta_time)
	{
		Enemy::Update(delta_time);
		animator.Update(delta_time);

		switch (state)
		{
		case State::Egg:
		{
			UpdatePhysics(delta_time, gravity_force / 2);
			UpdateCollision(delta_time);
			
			if (collision_tag & CollisionTag::Left || collision_tag & CollisionTag::Right)
				speed.X = -speed.X;
			if (collision_tag & CollisionTag::Floor || collision_tag & CollisionTag::Cell)
			{
				speed.Y = 0;
				SetState(State::Normal);
			}

			break;
		}
		case State::Normal:
		{
			UpdatePhysics(delta_time, gravity_force / 2);
			UpdateCollision(delta_time);

			if (collision_tag & CollisionTag::Left || collision_tag & CollisionTag::Right)
				speed.X = -speed.X;
			if (collision_tag & CollisionTag::Floor || collision_tag & CollisionTag::Cell)
				speed.Y = 0;

			animator.FlipX(speed.X > 0);

			break;
		}
		case State::Died:
		{
			UpdatePhysics(delta_time, gravity_force / 2);
			break;
		}
		default:
			break;
		}
	}

	void Spinny::KickFromTop(Mario* mario)
	{
		mario->ReciveDamage();
	}

	void Spinny::KickFromBottom(Mario* mario)
	{
		SetState(State::Died);
	}

	void Spinny::TouchSide(Mario* mario)
	{
		mario->ReciveDamage();
	}

	bool Spinny::IsAlive() const
	{
		return state != State::Died;
	}

	void Spinny::SetState(State state)
	{
		this->state = state;

		switch (this->state)
		{
			case State::Normal:
			{
				animator.Play("walk");
				speed.X = run_speed;

				if (walk_direction == Vector::Right)
					speed = -speed;

				break;
			}
			case State::Died:
			{
				animator.Play("died");
				AddScoreToPlayer(400);
				sMarioGame->PlaySound("kick");

				break;
			}
			case State::Egg:
			{
				animator.Play("egg");

				break;
			}
		}
	}
}