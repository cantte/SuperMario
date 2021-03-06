#include "CastleFlag.h"
#include "MarioGame.h"
#include "Blocks.h"

using namespace GameEngine;

namespace GameObjects
{
	CastleFlag::CastleFlag()
	{
		animator.Create("normal", *sMarioGame->TextureManager().Get("Items"), { 0,148 }, { 32,32 }, 4, 1, 0.01f);
		SetSize({ 32, 32 });
	}

	void CastleFlag::LiftUp()
	{
		Enable();
		pos_y -= 64;
	}

	void CastleFlag::Draw(sf::RenderWindow* render_window)
	{
		animator.SetPosition(GetPosition());
		animator.Draw(render_window);
	}

	void CastleFlag::Update(int delta_time)
	{
		if (GetPosition().Y > pos_y)
			Move(Vector::Up * delta_time * 0.03f);
		animator.Update(delta_time);
	}

	void CastleFlag::Init()
	{
		Move(Vector::Down * 64);
		pos_y = GetPosition().Y;
		MoveUnderTo(GetParent()->FindObjectByType<Blocks>());
		Disable();
	}
}