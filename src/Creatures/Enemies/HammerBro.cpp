#include "HammerBro.h"
#include "Hammer.h"
#include "Mario.h"
#include "MarioGame.h"
#include "Blocks.h"

namespace Creatures
{
	HammerBro::HammerBro()
	{
		SetSize({ 32, 44 });

		const sf::Texture& texture = *sMarioGame->TextureManager().Get("Enemies");

		animator.Create("died", texture, { 96,160 + 48,32,-48 });
		animator.Create("walk", texture, Vector(96, 160), Vector(32, 48), 2, 1, 0.005f);
		animator.Create("walk_with_hammer", texture, Vector(160, 160), Vector(32, 48), 2, 1, 0.005f);
		animator.Play("walk_with_hammer");

		speed.X = run_speed;
	}

	void HammerBro::Draw(sf::RenderWindow* render_window)
	{
		animator.SetPosition(GetPosition());
		animator.Draw(render_window);
	}

	void HammerBro::Update(int delta_time)
	{
		Enemy::Update(delta_time);

		if (speed.Y == 0)
			animator.Update(delta_time);

		switch (state)
		{
			case State::Normal:
			{
				UpdatePhysics(delta_time, gravity_force / 2);

				if (collision_on)
				{
					UpdateCollision(delta_time);

					if (collision_tag & CollisionTag::Left || collision_tag & CollisionTag::Right)
						speed.X = -speed.X;

					if (collision_tag & CollisionTag::Floor || collision_tag & CollisionTag::Cell)
						speed.Y = 0;
				}

				if (std::abs(GetPosition().X - center_x) > walk_amplitude)
					speed.X = -speed.X;

				direction = (mario->GetPosition().X < GetPosition().X) ? Vector::Left : Vector::Right;
				animator.FlipX(direction == Vector::Right);

				jump_timer += delta_time;
				if (jump_timer > jump_rate)
				{
					if (jump_direction == Vector::Up && !IsCanJumpUp() && IsCanJumpDown())
						jump_direction = Vector::Down;
					if (jump_direction == Vector::Down && !IsCanJumpDown())
						jump_direction = Vector::Up;

					if (jump_direction == Vector::Up)
						speed += Vector::Up * 0.5f;
					else
					{
						speed += Vector::Up * 0.25;
						drop_off_height = GetPosition().Y + GetBounds().Height + 32.f;
					}

					collision_on = false;
					jump_timer = 0;
				}

				if (!collision_on)
				{
					if (jump_direction == Vector::Up)
					{
						if (speed.Y > 0)
							collision_on = true;
					}
					else
					{
						if (GetPosition().Y > drop_off_height)
							collision_on = true;
					}
				}

				fire_timer += delta_time;
				if (fire_timer < fire_rate)
				{
					if (!hammer)
					{
						hammer = new Hammer(mario);
						GetParent()->AddObject(hammer);
						animator.Play("walk_with_hammer");
					}

					const Vector hand_off_set = { -3 * direction.X, -22.f };
					hammer->SetPosition(GetPosition() + hand_off_set);
				}
				else
				{
					hammer->ThrowAway({ direction.X * 0.15f, -0.55f });
					hammer = nullptr;
					fire_timer = 0;
					animator.Play("walk");
				}

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
	}

	void HammerBro::OnActivated()
	{
		center_x = GetPosition().X;
		fire_timer = rand() % 500;
		jump_timer = rand() % int(jump_rate / 2);
	}

	void HammerBro::KickFromTop(Mario* mario)
	{
		SetState(State::Died);
	}

	void HammerBro::KickFromBottom(Mario* mario)
	{
		SetState(State::Died);
	}

	void HammerBro::TouchSide(Mario* mario)
	{
		if (state == State::Normal)
			mario->ReciveDamage();
	}

	bool HammerBro::IsAlive() const
	{
		return state != State::Died;
	}

	bool HammerBro::IsCanJumpUp() const
	{
		Vector begin_point = blocks->ToBlockCoordinates(GetBounds().Center(), true);
		Vector end_point = blocks->TraceLine(begin_point, Vector::Up);

		if (end_point == begin_point)
			return false;

		if (end_point.Y == 0)
			return false;

		if (std::abs(end_point.Y - begin_point.Y) != 2 &&
			blocks->IsCollidableBlock(end_point + Vector::Up * 2))
			return false;
		return true;
	}

	bool HammerBro::IsCanJumpDown() const
	{
		Vector begin_point = blocks->ToBlockCoordinates(GetBounds().Center()) + Vector::Down * 2;

		if (blocks->IsCollidableBlock(begin_point))
			return false;
		return true;
	}

	void HammerBro::SetState(State state)
	{
		this->state = state;
		if (this->state == State::Died)
		{
			animator.Play("died");
			speed.Y = 0;

			if (hammer)
			{
				GetParent()->RemoveObject(hammer);
				hammer = nullptr;
			}

			AddScoreToPlayer(1000);
			sMarioGame->PlaySound("kick");
		}
	}
}