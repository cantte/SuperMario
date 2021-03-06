#include "Bowser.h"
#include "Mario.h"
#include "MarioGame.h"
#include "FireBall.h"

namespace Creatures
{
	Bowser::Bowser()
	{
		SetSize({ 84, 80 });

		sf::Texture& texture = *sMarioGame->TextureManager().Get("Bowser");
		animator.Create("walk", texture, { 0,0 }, { 84,80 }, 6, 1, anim_speed, AnimType::ForwardCycle);
		animator.Create("died", texture, { 0,80,84,-80 });
		animator.Create("turn", texture, { 381,122 }, { 74,85 }, 2, 1, anim_speed / 2, AnimType::ForwardStop);
		animator.Create("middle_fire", texture, { 0,167 }, { 91,100 }, 4, 1, anim_speed, AnimType::ForwardStop);
		animator.Create("land_fire", texture, { 0,267 }, { 92,97 }, 6, 1, anim_speed, AnimType::ForwardStop);
		animator.Create("pre_jump", texture, { 0,80 }, { 91,79 }, 2, 1, anim_speed, AnimType::ForwardStop);
		animator.Create("up_jump", texture, { 182,80,84,87 });
		animator.Create("down_jump", texture, { 266,80, 84,87 });
		animator.Get("middle_fire")->SetOrigin(Vector::Down * 16);
		animator.Get("land_fire")->SetOrigin(Vector::Down * 16);
		animator.Get("turn")->SetOrigin(Vector::Down * 5);

		speed.X = run_speed;
	}

	void Bowser::Draw(sf::RenderWindow* render_window)
	{
		animator.SetPosition(GetPosition());
		animator.Draw(render_window);
	}

	void Bowser::Update(int delta_time)
	{
		Enemy::Update(delta_time);
		animator.Update(delta_time);

		if (state == State::Walk || state == State::Jump)
		{
			UpdatePhysics(delta_time, gravity_force / 2);
			UpdateCollision(delta_time);

			if (collision_tag & CollisionTag::Left || collision_tag & CollisionTag::Right)
				speed.X = -speed.X;

			if (collision_tag & CollisionTag::Floor || collision_tag & CollisionTag::Cell)
				speed.Y = 0;
		}

		switch (state)
		{
			case State::Walk:
			{
				if (std::abs(GetPosition().X - center_x) > walk_amplitude)
				{
					speed.X = -speed.X;
					Move({ (walk_amplitude - std::abs(GetPosition().X - center_x)) * math::sign(-speed.X), 0.f });
				}

				Vector old_direction = direction;
				direction = (GetMario()->GetPosition().X < GetPosition().X) ? Vector::Left : Vector::Right;

				if (old_direction != direction)
				{
					SetState(State::Turn);
					break;
				}

				animator.FlipX(direction == Vector::Right);

				delay_timer -= delta_time;
				if (delay_timer < 0)
				{
					int d = rand() % 3;
					if (d == 0) SetState(State::PreJump);
					if (d == 1) SetState(State::MiddleFire);
					if (d == 2) SetState(State::LandFire);
				}

				break;
			}
			case State::Turn:
			{
				delay_timer -= delta_time;
				if (delay_timer < 0)
					SetState(State::Walk);

				break;
			}
			case State::Jump:
			{
				if (speed.Y > 0)
					animator.Play("down_jump");
				else if (speed.Y < 0)
					animator.Play("up_jump");

				if (speed.Y > 0 && old_speed.Y < 0)
				{
					GetParent()->AddObject(new FireBall(GetBounds().Center() + direction * 50 + Vector::Down * 20, direction * 0.13f));
					sMarioGame->PlaySound("bowser_fire");
				}

				old_speed.Y = speed.Y;

				if (collision_tag & CollisionTag::Floor)
					SetState(State::Walk);

				break;
			}
			case State::PreJump:
			{
				delay_timer -= delta_time;
				if (delay_timer < 0)
					SetState(State::Jump);

				break;
			}
			case State::MiddleFire:
			{
				delay_timer -= delta_time;
				if (delay_timer < 0)
				{
					GetParent()->AddObject(new FireBall(GetBounds().Center() + direction * 50 + Vector::Up * 10, direction * 0.13f));
					sMarioGame->PlaySound("bowser_fire");
					SetState(State::Walk);
				}

				break;
			}
			case State::LandFire:
			{
				delay_timer -= delta_time;
				if (delay_timer < 0)
				{
					GetParent()->AddObject(new FireBall(GetBounds().Center() + direction * 50 + Vector::Down * 10, direction * 0.13f));
					sMarioGame->PlaySound("bowser_fire");
					SetState(State::Walk);
				}

				break;
			}
			case State::NoBridge:
			{
				delay_timer -= delta_time;
				if (delay_timer < 0)
					SetState(State::Fall);

				break;
			}
			case State::Fall:
			case State::Died:
			{
				UpdatePhysics(delta_time, gravity_force / 2);
				break;
			}
			default:
				break;
		}
	}

	void Bowser::OnActivated()
	{
		center_x = GetPosition().X;
	}

	void Bowser::KickFromTop(Mario* mario)
	{
		mario->ReciveDamage();
	}

	void Bowser::KickFromBottom(Mario* mario)
	{
		lives--;
		if (lives < 0)
			SetState(State::Died);
	}

	void Bowser::TouchSide(Mario* mario)
	{
		mario->ReciveDamage();
	}

	bool Bowser::IsAlive() const
	{
		return true;
	}

	void Bowser::NoBridge()
	{
		SetState(State::NoBridge);
	}

	void Bowser::SetState(State state)
	{
		if (this->state == state)
			return;

		switch (state)
		{
			case State::Walk:
			{
				speed.X = old_speed.X;
				animator.Play("walk");
				delay_timer = 2000;

				break;
			}
			case State::Turn:
			{
				animator.Play("turn");
				delay_timer = 400;

				break;
			}
			case State::Jump:
			{
				speed = 0.4f * Vector::Up;

				break;
			}
			case State::PreJump:
			{
				old_speed.X = speed.X;
				old_speed.Y = -1;
				speed.X = 0;

				animator.Play("pre_jump");
				delay_timer = 300;

				break;
			}
			case State::MiddleFire:
			{
				old_speed.X = speed.X;
				speed.X = 0;
				animator.Play("middle_fire");
				delay_timer = 500;

				break;
			}
			case State::LandFire:
			{
				old_speed.X = speed.X;
				speed.X = 0;

				animator.Play("land_fire");
				delay_timer = 700;

				break;
			}
			case State::NoBridge:
			{
				animator.Play("walk");
				animator.Get("walk")->SetSpeed(anim_speed * 2.5f);
				delay_timer = 1000;

				break;
			}
			case State::Fall:
			{
				animator.Get("walk")->SetSpeed(0);
				sMarioGame->PlaySound("bowser_falls");
				speed = Vector::Zero;

				break;
			}
			case State::Died:
			{
				animator.Play("died");
				sMarioGame->PlaySound("bowser_falls");
				speed = Vector::Zero;

				break;
			}
			default:
				break;
		}

		this->state = state;
	}
}