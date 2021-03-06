#include "Lakity.h"
#include "MarioGame.h"
#include "Mario.h"
#include "Spinny.h"
#include "GameScene.h"

namespace Creatures
{
	Lakity::Lakity()
	{
		SetName("Lakity");
		SetSize({ 32, 48 });

		const sf::Texture& texture = *sMarioGame->TextureManager().Get("Enemies");
		animator.Create("fire", texture, { 0,128,32,48 });
		animator.Create("fly", texture, { 32,128,32,48 });
		animator.Create("died", texture, { 32,128 + 48,32,-48 });

		SetState(State::Normal);
	}

	void Lakity::Draw(sf::RenderWindow* render_window)
	{
		animator.SetPosition(GetPosition());
		animator.Draw(render_window);
	}

	void Lakity::Update(int delta_time)
	{
		Enemy::Update(delta_time);

		switch (state)
		{
			case State::Normal:
			{
				float diff_x = GetMario()->GetPosition().X - GetPosition().X;
				speed.X += math::sign(diff_x) * sqrt(std::abs(diff_x)) / 4000;
				speed.X = math::clamp(speed.X, -0.35f, 0.35f);

				Move(speed * delta_time);

				fire_timer += delta_time;
				if (fire_timer > fire_rate)
				{
					Vector fly_direction = (GetMario()->GetPosition().X > GetPosition().X) ? Vector::Right : Vector::Left;
					Spinny* spinny = new Spinny(GetPosition() - Vector(0, 10), Vector(-0.05f * fly_direction.X, -0.2f), fly_direction);
					GetParent()->AddObject(spinny);
					fire_timer = 0;
					animator.Play("fly");
				}

				if (fire_timer > fire_rate * 0.8f)
					animator.Play("fire");

				break;
			}
			case State::Died:
			{
				UpdatePhysics(delta_time, gravity_force / 2);
				break;
			}
			case State::RunAway:
			{
				Move(speed * delta_time);
				died_timer += delta_time;

				if (died_timer > 2000)
					GetParent()->RemoveObject(this);

				break;
			}
			default:
				break;
		}
	}

	void Lakity::KickFromTop(Mario* mario)
	{
		SetState(State::Died);
	}

	void Lakity::KickFromBottom(Mario* mario)
	{
		SetState(State::Died);
	}

	void Lakity::TouchSide(Mario* mario)
	{
		mario->ReciveDamage();
	}

	bool Lakity::IsAlive() const
	{
		return state != State::Died;
	}

	void Lakity::Escape(const Vector& escape_direction)
	{
		speed.X = escape_direction.X * 0.2f;
		SetState(State::RunAway);
	}

	void Lakity::Init()
	{
		Enemy::Init();
	}

	void Lakity::SetState(State state)
	{
		this->state = state;

		if (state = State::Died)
		{
			animator.Play("died");
			speed = Vector::Zero;
			AddScoreToPlayer(1200);
			sMarioGame->PlaySound("kick");
		}
		else if (state == State::Normal)
			animator.Play("fly");
	}

	void LakitySpawner::Update(int delta_time)
	{
		GameObject::Update(delta_time);

		lakity_checker_timer += delta_time;

		if (lakity_checker_timer > check_interval)
		{
			lakity = GetParent()->FindObjectByName<Lakity>("Lakity");
			Rect camera_rect = GetParent()->CastTo<GameScene>()->CameraRect();

			if (!lakity)
			{
				if (GetBounds().IsContainByX(mario->GetPosition()))
				{
					lakity = new Lakity();
					lakity->SetPosition(camera_rect.Left - 32, 64);
					GetParent()->AddObject(lakity);
				}
			}

			lakity_checker_timer = 0;
		}

		if (lakity)
		{
			Rect camera_rect = GetParent()->CastTo<GameScene>()->CameraRect();

			if (lakity->GetPosition().X > GetBounds().Right() + camera_rect.Size().X / 2)
			{
				lakity->Escape(Vector::Left);
				lakity = nullptr;
			}
			else if (lakity->GetPosition().X < GetBounds().Left - camera_rect.Size().X / 2)
			{
				lakity->Escape(Vector::Right);
				lakity = nullptr;
			}
		}
	}

	void LakitySpawner::OnActivated()
	{
		SetSize({ GetProperty("Width").AsFloat(), GetProperty("Height").AsFloat() });
	}

	void LakitySpawner::Init()
	{
		mario = GetParent()->FindObjectByName<Mario>("Mario");
	}
}