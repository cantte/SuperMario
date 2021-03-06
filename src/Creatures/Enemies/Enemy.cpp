#include "Enemy.h"
#include "Blocks.h"
#include "Mario.h"
#include "MarioGame.h"

namespace Creatures
{
	void Enemy::Fired(Mario* mario)
	{
		KickFromBottom(mario);
	}

	void Enemy::Update(int delta_time)
	{
		CheckFallUndergound();
	}

	void Enemy::Init()
	{
		mario = GetParent()->FindObjectByName<Mario>("Mario");
		blocks = GetParent()->FindObjectByName<GameObjects::Blocks>("Blocks");
		assert(mario && blocks);
	}

	void Enemy::AddScoreToPlayer(int score)
	{
		sMarioGame->AddScore(score, GetBounds().Center());
	}

	void Enemy::CheckNextTileUnderFoots()
	{
		if (speed.Y == 0)
		{
			Vector own_center = GetBounds().Center();
			Vector opposite_vector = math::sign(speed.X) * Vector::Right;

			bool is_next_under_foot = blocks->IsCollidableBlock(blocks->ToBlockCoordinates(own_center + 20 * opposite_vector + 32 * Vector::Down));
			bool is_prev_under_foot = blocks->IsCollidableBlock(blocks->ToBlockCoordinates(own_center - 60 * opposite_vector + 32 * Vector::Down));
			bool is_prev_back = blocks->IsCollidableBlock(blocks->ToBlockCoordinates(own_center - 50 * opposite_vector));
			bool is_next_back = blocks->IsCollidableBlock(blocks->ToBlockCoordinates(own_center + 50 * opposite_vector));

			if ((!is_next_under_foot && !is_prev_back) && (is_next_under_foot || is_prev_under_foot))
				speed.X = -speed.X;
		}
	}

	void Enemy::CheckCollideOtherCharasters()
	{
		auto enemies = GetParent()->FindObjectsByType<Enemy>();
		for (auto enemy : enemies)
		{
			if (enemy != this && enemy->IsAlive() &&
				enemy->GetBounds().IsIntersect(GetBounds()))
			{
				bool is_enemy_in_bullet_state_also = enemy->IsInBulletState();
				enemy->KickFromBottom(nullptr);

				if (is_enemy_in_bullet_state_also)
				{
					KickFromBottom(nullptr);
					break;
				}
			}
		}
	}

	void Enemy::CheckFallUndergound()
	{
		if (GetPosition().Y > 1000)
			GetParent()->RemoveObject(this);
	}

	void Enemy::UpdateCollision(float delta_time)
	{
		collision_tag = CollisionTag::None;
		SetPosition(blocks->CollisionResponse(GetBounds(), speed, delta_time, collision_tag));
	}

	void Enemy::UpdatePhysics(float delta_time, float gravity)
	{
		speed += Vector::Down * gravity_force * delta_time;
		Move(delta_time * speed);
	}
}