#include "Goomba.h"
#include "Mario.h"
#include "MarioGame.h"
#include "GameScene.h"

namespace Creatures
{
	Goomba::Goomba()
	{
		SetSize({ 32, 32 });
		const sf::Texture& texture = *sMarioGame->TextureManager().Get("Enemies");
		animator.Create("walk", texture, { { 0,0,32,32 },{ 32,0,32,32 } }, 0.005f);
		animator.Create("cramped", texture, { 64,0,32,32 });
		animator.Create("fall", texture, { 0,32,32,-32 });
		animator.SetSpriteOffset("cramped", 0, { 0,8 });
	}

	void Goomba::Draw(sf::RenderWindow* render_window)
	{
		animator.SetPosition(GetPosition());
		animator.Draw(render_window);
	}

	void Goomba::Update(int delta_time)
	{
		Enemy::Update(delta_time);

		switch (state)
		{
			case Deactivated:
			{
				auto scene = GetParent()->CastTo<GameScene>();
				Rect camera_rect = scene->CameraRect();

				if (std::abs(GetPosition().X - camera_rect.Center().X) < camera_rect.Width / 2)
					SetState(State::Normal);

				break;
			}
			case Normal:
			{
				UpdatePhysics(delta_time, gravity_force);
				UpdateCollision(delta_time);

				if ((collision_tag & CollisionTag::Floor) || (collision_tag & CollisionTag::Cell))
					speed.Y = 0;
			
				if ((collision_tag & CollisionTag::Left) || (collision_tag & CollisionTag::Right))
					speed.X = -speed.X;

				break;
			}
			case Cramped:
			{
				timer += delta_time;
				if (timer > 3000)
					GetParent()->RemoveObject(this);

				break;
			}
			case Died:
			{
				UpdatePhysics(delta_time, gravity_force);
				break;
			}
			default:
				break;
		}

		switch (state)
		{
			case Normal:
			{
				animator.Play("walk");
				animator.Update(delta_time);
				break;
			}
			case Cramped:
			{
				animator.Play("cramped");
				break;
			}
			case Died:
			{
				animator.Play("fall");
				break;
			}
			default:
				break;
		}
	}

	void Goomba::KickFromTop(Mario* mario)
	{
		SetState(State::Cramped);
	}

	void Goomba::KickFromBottom(Mario* mario)
	{
		if (state != Died)
		{
			SetState(State::Died);
			speed += 0.4f * Vector::Up;
		}
	}

	void Goomba::TouchSide(Mario* mario)
	{
		if (state == State::Normal)
			mario->ReciveDamage();
	}

	bool Goomba::IsAlive() const
	{
		return state == Normal;
	}

	void Goomba::SetState(State state)
	{
		this->state = state;

		switch (state)
		{
			case Normal:
			{
				speed.X = run_speed;
				break;
			}
			case Cramped:
			{
				speed.X = 0;
				AddScoreToPlayer(100);
				sMarioGame->PlaySound("stomp");
				break;
			}
			case Died:
			{
				speed.X = 0;
				AddScoreToPlayer(100);
				sMarioGame->PlaySound("kick");
				break;
			}
			default:
				break;
			}
	}
}