#ifndef ENEMY_H
#define ENEMY_H

#include "GameObject.h"
#include "GameEngine.h"

namespace GameObjects
{
	class Blocks;
}

using namespace GameEngine;

namespace Creatures
{
	class Mario;

	class Enemy : public GameObjects::GameObject
	{
	public:
		virtual void KickFromTop(Mario* mario) = 0;
		virtual void KickFromBottom(Mario* mario) = 0;
		virtual bool IsAlive() const = 0;
		virtual void TouchSide(Mario* mario) = 0;
		virtual bool IsInBulletState() const { return false; }
		virtual void Fired(Mario* mario);
		void Update(int delta_time);

	protected:
		CollisionTag collision_tag;
		const float gravity_force = 0.0015f;
		Vector speed;
		Vector direction = Vector::Left;
		Mario* mario = nullptr;
		GameObjects::Blocks* blocks = nullptr;
		const float run_speed = -0.05f;

		virtual void Init() override;
		Mario* GetMario() { return mario; }

		void AddScoreToPlayer(int score);
		void CheckNextTileUnderFoots();
		void CheckCollideOtherCharasters();
		void CheckFallUndergound();
		void UpdateCollision(float delta_time);
		void UpdatePhysics(float delta_time, float gravity);
	};
}

#endif // !ENEMY_H