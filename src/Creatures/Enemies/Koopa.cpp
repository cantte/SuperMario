#include "Koopa.h"
#include "Mario.h"
#include "MarioGame.h"
#include "GameScene.h"

namespace Creatures
{
	Koopa::Koopa()
	{
		SetSize({ 32, 48 });

		const sf::Texture& texture = *sMarioGame->TextureManager().Get("Enemies");
		
		animator.Create("walk", texture, { { 0,32,32,48 },{ 32,32,32,48 } }, 0.005f);
		animator.Create("flying", texture, { 224,32 }, { 32, 48 }, 2, 1, 0.005f);
		animator.Create("climb", texture, { { 64,48,32,32 },{ 192,48,32,32 } }, 0.005f);
		animator.Create("hidden", texture, { 64,48,32,32 });
		animator.Create("bullet", texture, { 64, 48 }, { 32, 32 }, 4, 1, 0.01f);
		animator.Create("fall", texture, { 0,80, 32, -48 });
	}

	void Koopa::KickFromTop(Mario* mario)
	{
		switch (state)
		{
			case Jumping:
			case Levitating:
			{
				speed.Y = 0;
				SetState(State::Normal);
				AddScoreToPlayer(100);
				sMarioGame->PlaySound("stomp");
				break;
			}
			case Normal:
			{
				if (speed.Y == 0)
				{
					SetState(State::Hidden);
					AddScoreToPlayer(100);
					sMarioGame->PlaySound("stomp");
				}
				break;
			}
			case Hidden:
			case Climb:
			{
				SetState(State::Bullet);
				speed.X = (mario->GetBounds().Center().X > GetBounds().Center().X) ? -std::abs(run_speed) * 6 : std::abs(run_speed) * 6;

				AddScoreToPlayer(400);
				sMarioGame->PlaySound("kick");
				break;
			}
			case Bullet:
			{
				SetState(Hidden);
				break;
			}
			default:
				break;
		}
	}

	void Koopa::KickFromBottom(Mario* mario)
	{
		if (state != State::Died)
		{
			SetState(State::Died);

			AddScoreToPlayer(500);
			sMarioGame->PlaySound("kick");
		}
	}

	bool Koopa::IsAlive() const
	{
		return state != State::Died;
	}

	void Koopa::TouchSide(Mario* mario)
	{
		if (state == State::Hidden || state == State::Climb)
		{
			SetState(State::Bullet);
			speed.X = (mario->GetBounds().Center().X > GetBounds().Center().X) ? -std::abs(run_speed) * 6 : std::abs(run_speed) * 6;

			Move(14 * Vector::Right * math::sign(speed.X));
			AddScoreToPlayer(400);
			sMarioGame->PlaySound("kick");
		}
		else if (state == State::Normal || state == State::Bullet)
			mario->ReciveDamage();
	}

	void Koopa::Draw(sf::RenderWindow* render_window)
	{
		animator.SetPosition(GetPosition());
		animator.Draw(render_window);
	}

	void Koopa::Update(int delta_time)
	{
		Enemy::Update(delta_time);

		if (state == State::Deactivated)
		{
			Rect camera_rect = GetParent()->CastTo<GameScene>()->CameraRect();

			if (std::abs(GetPosition().X - camera_rect.Center().X) < camera_rect.Width / 2)
			{
				if (flying_mode == 1)
					SetState(State::Jumping);
				else if (flying_mode == 2)
					SetState(State::Levitating);
				else
					SetState(State::Normal);
			}
		}
		else
		{
			if (state != State::Levitating)
				UpdatePhysics(delta_time, gravity_force);

			if (state != State::Died && state != State::Levitating)
			{
				UpdateCollision(delta_time);

				if ((collision_tag & CollisionTag::Left) || (collision_tag & CollisionTag::Right))
					speed.X = -speed.X;

				if (collision_tag & CollisionTag::Floor)
				{
					if (state == State::Jumping)
						speed.Y = -0.4f;
					else
						speed.Y = 0.f;
				}

				if (collision_tag & CollisionTag::Cell)
					speed.Y = 0;
			}

			switch (state)
			{
				case State::Normal:
				{
					CheckNextTileUnderFoots();
					break;
				}
				case State::Jumping:
				{
					speed += Vector::Up * delta_time * gravity_force / 2;
					break;
				}
				case State::Levitating:
				{
					timer += delta_time;
					SetPosition(initial_pos + Vector::Up * sin(timer / 800.f) * 50);
					break;
				}
				case State::Hidden:
				{
					timer += delta_time;
					if (timer > 3000)
						SetState(State::Climb);
					break;
				}
				case State::Climb:
				{
					timer += delta_time;
					if (timer > 5000)
						SetState(State::Normal);
					break;
				}
				case State::Bullet:
				{
					CheckCollideOtherCharasters();
					break;
				}
				default:
					break;
			}
		}

		if (state == State::Bullet && ((collision_tag & CollisionTag::Left) || (collision_tag & CollisionTag::Right)))
			GetParent()->CastTo<GameScene>()->PlaySoundAtPoint("bump", GetBounds().Center());

		animator.FlipX(speed.X > 0);
		animator.Update(delta_time);
	}

	bool Koopa::IsInBulletState() const
	{
		return state == State::Bullet;
	}

	void Koopa::OnActivated()
	{
		flying_mode = GetProperty("Flying").AsInt();
	}

	void Koopa::SetState(State state)
	{
		timer = 0;

		if (this->state != state)
		{
			this->state = state;

			switch (this->state)
			{
				case State::Jumping:
				{
					animator.Play("flying");
					speed.X = run_speed;

					break;
				}
				case State::Levitating:
				{
					animator.Play("flying");
					speed.X = 0;
					initial_pos = GetPosition();

					break;
				}
				case State::Normal:
				{
					speed.X = run_speed;
					animator.Play("walk");

					Move(GetBounds().Size() - full_size);
					SetSize(full_size);

					break;
				}
				case State::Hidden:
				{
					Move(GetBounds().Size() - hidden_size);
					SetSize(hidden_size);
					speed.X = 0;
					animator.Play("hidden");

					break;
				}
				case State::Climb:
				{
					Move(GetBounds().Size() - hidden_size);
					SetSize(hidden_size);
					animator.Play("climb");

					break;
				}
				case State::Died:
				{
					speed.Y = -0.4f;
					animator.Play("fall");

					break;
				}
			}

			timer = 0;
		}
	}
}