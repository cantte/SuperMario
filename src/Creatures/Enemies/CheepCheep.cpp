#include "CheepCheep.h"
#include "Mario.h"
#include "MarioGame.h"
#include "GameScene.h"
#include "Blocks.h"

namespace Creatures
{
	CheepCheep::CheepCheep()
	{
		SetSize({ 32, 32 });
		speed = Vector::Left * 0.05f;

		const sf::Texture& texture = *sMarioGame->TextureManager().Get("Enemies");
		animator.Create("fly", texture, Vector(0, 176), Vector(32, 32), 2, 1, 0.005f);
		animator.Create("died", texture, { 0, 176 + 32, 32, -32 });

		SetState(State::Underwater);
	}

	CheepCheep::CheepCheep(const Vector& initial_pos, const Vector& initial_speed)
	{
		SetSize({ 32, 32 });
		speed = initial_speed;
		SetPosition(initial_pos);

		const sf::Texture& texture = *sMarioGame->TextureManager().Get("Enemies");
		animator.Create("fly", texture, Vector(0, 176), Vector(32, 32), 2, 1, 0.005f);
		animator.Create("died", texture, { 0, 176 + 32, 32, -32 });
		
		SetState(State::Normal);
	}

	void CheepCheep::Draw(sf::RenderWindow* render_window)
	{
		animator.SetPosition(GetPosition());
		animator.Draw(render_window);
	}

	void CheepCheep::Update(int delta_time)
	{
		Enemy::Update(delta_time);

		switch (state)
		{
			case State::Normal:
			{
				UpdatePhysics(delta_time, gravity_force * 0.4f);
				animator.Update(delta_time);

				break;
			}
			case State::Underwater:
			{
				if (std::abs(GetMario()->GetPosition().X - GetPosition().X) < sMarioGame->ScreenSize().X / 2)
				{
					Move(speed * delta_time);
					animator.Update(delta_time);
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

	void CheepCheep::KickFromTop(Mario* mario)
	{
		SetState(State::Died);
	}

	void CheepCheep::KickFromBottom(Mario* mario)
	{
		SetState(State::Died);
	}

	void CheepCheep::TouchSide(Mario* mario)
	{
		mario->ReciveDamage();
	}

	bool CheepCheep::IsAlive() const
	{
		return state != State::Died;
	}

	void CheepCheep::SetState(State state)
	{
		this->state = state;

		if (this->state == State::Normal)
		{
			animator.Play("fly");
			animator.FlipX(speed.X > 0);
		}
		else if (this->state == State::Died)
		{
			speed = Vector::Zero;
			animator.Play("died");
			AddScoreToPlayer(200);
			sMarioGame->PlaySound("kick");
		}
	}

	CheepCheepSpawner::CheepCheepSpawner()
	{

	}

	void CheepCheepSpawner::Update(int delta_time)
	{
		spawn_timer += delta_time;

		if (spawn_timer > spawn_interval&& GetBounds().IsContainByX(mario->GetPosition()))
		{
			int camera_width = GetParent()->CastTo<GameScene>()->CameraRect().Width;
			int x = rand() % int(camera_width) - camera_width / 2 + mario->GetPosition().X;

			Vector direction = (mario->GetPosition().X < x) ? Vector::Left : Vector::Right;
			
			GetParent()->AddObject(new CheepCheep({ x, map_height + 32 }, { direction.X * spawn_speed.X, spawn_speed.Y }));
			spawn_timer = 0;
		}
	}

	void CheepCheepSpawner::OnActivated()
	{
		SetSize({ GetProperty("width").AsFloat(), GetProperty("Height").AsFloat() });
	}

	void CheepCheepSpawner::Init()
	{
		map_height = GetParent()->FindObjectByName<GameObjects::Blocks>("Blocks")->Height();
		mario = GetParent()->FindObjectByName<Mario>("Mario");
	}
}