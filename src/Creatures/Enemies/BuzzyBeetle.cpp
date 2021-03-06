#include "BuzzyBeetle.h"
#include "Mario.h"
#include "MarioGame.h"
#include "GameScene.h"

namespace Creatures
{
	BuzzyBeetle::BuzzyBeetle()
	{
		SetSize({ 32, 32 });

		const sf::Texture& texture = *sMarioGame->TextureManager().Get("Enemies");
		animator.Create("walk", texture, { { 96,0,32,32 },{ 128,0,32,32 } }, 0.005f);
		animator.Create("hidden", texture, { 160,0,32,32 });
		animator.Create("bullet", texture, { 160, 0 }, { 32, 32 }, 4, 1, 0.01f);
		animator.Create("fall", texture, { 96,32, 32, -32 });

		SetState(State::Normal);
	}

	void BuzzyBeetle::KickFromTop(Mario* mario)
	{
		if (state == State::Normal)
		{
			if (speed.Y == 0)
			{
				SetState(State::Hidden);
				AddScoreToPlayer(100);
				sMarioGame->PlaySound("stomp");
			}
		}
		else if (state == State::Hidden)
		{
			SetState(State::Bullet);
			speed.X = (mario->GetBounds().Center().X > GetBounds().Center().X) ? -std::abs(run_speed) * 6 : std::abs(run_speed) * 6;
			AddScoreToPlayer(400);
			sMarioGame->PlaySound("kick");
		}
		else if (state == State::Bullet)
			SetState(State::Hidden);
	}

	void BuzzyBeetle::KickFromBottom(Mario* mario)
	{
		if (state != State::Died)
		{
			SetState(State::Died);
			speed.Y = -0.4f;
			timer = 0;
			AddScoreToPlayer(800);
			sMarioGame->PlaySound("kick");
		}
	}

	bool BuzzyBeetle::IsAlive() const
	{
		return state != State::Died;
	}

	void BuzzyBeetle::TouchSide(Mario* mario)
	{
		if (state == State::Hidden)
		{
			SetState(State::Bullet);
			speed.X = (mario->GetBounds().Center().X > GetBounds().Center().X) ? -std::abs(run_speed) * 6 : std::abs(run_speed) * 6;
			Move(14 * Vector::Right * math::sign(speed.X));
			sMarioGame->PlaySound("kick");
		}
		else if (state == State::Normal || state == State::Bullet)
			mario->ReciveDamage();
	}

	void BuzzyBeetle::Draw(sf::RenderWindow* render_window)
	{
		animator.SetPosition(GetPosition());
		animator.Draw(render_window);
	}

	void BuzzyBeetle::Update(int delta_time)
	{
		Enemy::Update(delta_time);

		switch (state)
		{
			case State::Deactivated:
			{
				auto scene = GetParent()->CastTo<GameScene>();
				Rect camera_rect = scene->CameraRect();

				if (std::abs(GetPosition().X - camera_rect.Center().X) < camera_rect.Width / 2)
					SetState(State::Normal);

				break;
			}
			case State::Normal:
			{
				UpdatePhysics(delta_time, gravity_force);
				UpdateCollision(delta_time);

				if (collision_tag & CollisionTag::Left || collision_tag & CollisionTag::Right)
					speed.X = -speed.X;

				if (collision_tag & CollisionTag::Floor || collision_tag & CollisionTag::Cell)
					speed.Y = 0;

				CheckNextTileUnderFoots();

				break;
			}
			case State::Hidden:
			{
				timer += delta_time;

				if (timer > 5000)
				{
					SetState(State::Normal);
					timer = 0;
				}

				UpdatePhysics(delta_time, gravity_force);
				UpdateCollision(delta_time);

				if (collision_tag & CollisionTag::Left || collision_tag & CollisionTag::Right)
					speed.X = -speed.X;

				if (collision_tag & CollisionTag::Floor || collision_tag & CollisionTag::Cell)
					speed.Y = 0;

				break;
			}
			case State::Bullet:
			{
				UpdatePhysics(delta_time, gravity_force);
				UpdateCollision(delta_time);

				if (collision_tag & CollisionTag::Left || collision_tag & CollisionTag::Right)
				{
					speed.X = -speed.X;
					GetParent()->CastTo<GameScene>()->PlaySoundAtPoint("bump", GetBounds().Center());
				}

				if (collision_tag & CollisionTag::Floor || collision_tag & CollisionTag::Cell)
					speed.Y = 0;

				CheckCollideOtherCharasters();

				break;
			}
			case State::Died:
			{
				UpdatePhysics(delta_time, gravity_force);
				break;
			}
			default:
				break;
		}

		animator.FlipX(speed.X > 0);
		animator.Update(delta_time);
	}

	bool BuzzyBeetle::IsInBulletState() const
	{
		return state == State::Bullet;
	}

	void BuzzyBeetle::SetState(State state)
	{
		this->state = state;

		switch (this->state)
		{
			case State::Normal:
			{
				speed.X = run_speed;
				animator.Play("walk");

				break;
			}
			case State::Hidden:
			{
				speed.X = 0;
				animator.Play("hidden");

				break;
			}
			case State::Bullet:
			{
				speed.X = 6 * run_speed;
				animator.Play("bullet");

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