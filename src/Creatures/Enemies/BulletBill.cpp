#include "BulletBill.h"
#include "Mario.h"
#include "MarioGame.h"
#include "GameScene.h"
#include "Blocks.h"

namespace Creatures
{
	BulletBill::BulletBill(const Vector& initial_pos, const Vector& initial_speed)
	{
		SetSize({ 32, 32 });
		speed = initial_speed;
		SetPosition(initial_pos);

		const sf::Texture& texture = *sMarioGame->TextureManager().Get("Enemies");
		animator.Create("fly", texture, Vector(64, 112), Vector(32, 32), 1, 3, 0.005f);
		animator.Create("died", texture, { 64,176 + 32,32,-32 });

		SetState(State::Normal);
	}

	void BulletBill::Draw(sf::RenderWindow* render_window)
	{
		animator.SetPosition(GetPosition());
		animator.Draw(render_window);
	}

	void BulletBill::Update(int delta_time)
	{
		Enemy::Update(delta_time);

		if (state == State::Normal)
		{
			UpdatePhysics(delta_time, 0);
			animator.Update(delta_time);

			int camera_width = GetParent()->CastTo<GameScene>()->CameraRect().Width;
			int distance_to_mario = std::abs(GetPosition().X - GetMario()->GetPosition().X);

			if (distance_to_mario > camera_width)
				GetParent()->RemoveObject(this);
		}
		else
			UpdatePhysics(delta_time, gravity_force);
	}

	void BulletBill::KickFromTop(Mario* mario)
	{
		SetState(State::Died);
	}

	void BulletBill::KickFromBottom(Mario* mario)
	{
		SetState(State::Died);
	}

	void BulletBill::TouchSide(Mario* mario)
	{
		mario->ReciveDamage();
	}

	bool BulletBill::IsAlive() const
	{
		return state != State::Died;
	}

	void BulletBill::Init()
	{
		Enemy::Init();
	}

	void BulletBill::SetState(State state)
	{
		this->state = state;

		if (this->state == State::Normal)
		{
			animator.Play("fly");
			animator.FlipX(speed.X > 0);
		}
		else
		{
			speed = Vector::Zero;
			animator.Play("died");
			AddScoreToPlayer(1000);
			sMarioGame->PlaySound("kick");
		}
	}

	BulletBillSpawner::BulletBillSpawner()
	{
		spawn_timer = rand() % spawn_interval;
	}

	void BulletBillSpawner::Update(int delta_time)
	{
		int camera_width = GetParent()->CastTo<GameScene>()->CameraRect().Width;
		spawn_timer += delta_time;

		bool is_mario_close_enough = std::abs(mario->GetPosition().X - GetPosition().X) < camera_width;
		bool is_time_to_push = spawn_timer > spawn_interval;
		bool is_bullet_bill_beyond_tiled_map = IsBulletBillBeyondTiledMap();

		if (is_time_to_push && (is_mario_close_enough || is_bullet_bill_beyond_tiled_map))
		{
			Vector direction = (mario->GetPosition().X < GetPosition().X) ? Vector::Left : Vector::Right;
			Vector pos = GetPosition();

			if (is_bullet_bill_beyond_tiled_map)
			{
				int k = (pos.X < mario->GetPosition().X ? -1 : 1);
				pos.X = mario->GetPosition().X + k * sMarioGame->ScreenSize().X / 2;
			}

			GetParent()->AddObject(new BulletBill(pos, direction * bullet_speed));
			spawn_timer = 0;
			
			if (is_bullet_bill_beyond_tiled_map)
				spawn_timer = -4000 - rand() % 8000;

			sMarioGame->PlaySound("fireworks");
		}
	}

	void BulletBillSpawner::OnActivated()
	{

	}

	void BulletBillSpawner::Init()
	{
		mario = GetParent()->FindObjectByName<Mario>("Mario");
		blocks_width = GetParent()->FindObjectByName<GameObjects::Blocks>("Blocks")->Width();

		if (IsBulletBillBeyondTiledMap())
			spawn_timer = -rand() % 5000;
	}

	bool BulletBillSpawner::IsBulletBillBeyondTiledMap() const
	{
		return GetPosition().X < 0 || GetPosition().X > blocks_width;
	}

}