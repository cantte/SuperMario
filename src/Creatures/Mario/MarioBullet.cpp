#include "MarioBullet.h"
#include "MarioGame.h"
#include "Enemy.h"
#include "Blocks.h"

namespace Creatures
{
	MarioBullet::MarioBullet(const Vector& pos, const Vector& direction)
	{
		this->direction = direction;
		SetPosition(pos);
		speed = direction * bullet_speed;

		animator.Create("fly", *sMarioGame->TextureManager().Get("Mario"), { { 0,0,16,16 },{ 16,0,16,16 },{ 16,0,-16,16 },{ 16,16,16,-16 } }, 0.01f);
		animator.Create("splash", *sMarioGame->TextureManager().Get("Mario"), Vector(31, 0), Vector(16, 16), 3, 1, 0.02f, AnimType::ForwardBackwardCycle);
	}

	void MarioBullet::Draw(sf::RenderWindow* render_window)
	{
		animator.SetPosition(GetPosition());
		animator.Draw(render_window);
	}

	void MarioBullet::Update(int delta_time)
	{
		timer += delta_time;
		if (timer > 3000)
			GetParent()->RemoveObject(this);

		if (state == State::Fly)
		{
			speed += Vector::Down * gravity_force * delta_time;
			Move(speed * delta_time);

			Vector block = blocks->ToBlockCoordinates(GetPosition());
			if (blocks->IsBlockInBounds(block) && blocks->IsCollidableBlock(block))
			{
				Rect block_bounds = blocks->GetBlockBounds(block);
				float dw = std::min(std::abs(block_bounds.Left - GetPosition().X), std::abs(block_bounds.Right() - GetPosition().X));
				float dh = std::min(std::abs(block_bounds.Top - GetPosition().Y), std::abs(block_bounds.Bottom() - GetPosition().Y));

				if (dw < dh && (dw > 4 && dh > 4))
					SetState(State::Splash);
				else 
					speed.Y = -0.35f;
			}

			auto enemies = GetParent()->FindObjectsByType<Enemy>();
			for (auto enemy : enemies)
			{
				if (enemy->GetBounds().IsContain(GetPosition()))
				{
					enemy->Fired(nullptr);
					SetState(State::Splash);
					break;
				}
			}
		}
		else
		{
			if (timer > 250)
				GetParent()->RemoveObject(this);
		}

		animator.Update(delta_time);
	}

	void MarioBullet::SetState(State state)
	{
		this->state = state;

		if (state == State::Fly)
			animator.Play("fly");
		else if (state == State::Splash)
			animator.Play("splash");

		timer = 0;
	}

	void MarioBullet::Init()
	{
		blocks = GetParent()->FindObjectByName<GameObjects::Blocks>("Blocks");
	}
}