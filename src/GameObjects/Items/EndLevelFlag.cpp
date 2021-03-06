#include "EndLevelFlag.h"
#include "Mario.h"
#include "MarioState.h"
#include "MarioGame.h"
#include "Blocks.h"

using namespace Creatures;

namespace GameObjects
{
	EndLevelFlag::EndLevelFlag()
	{
		animator.Create("base", *sMarioGame->TextureManager().Get("Items"), { 0, 180 }, { 32, 32 }, 4, 1, 0.01f);
	}

	void EndLevelFlag::Draw(sf::RenderWindow* render_window)
	{
		animator.SetPosition(GetPosition());
		animator.Draw(render_window);
	}

	void EndLevelFlag::Update(int delta_time)
	{
		GameObject::Update(delta_time);

		if (!touched)
		{
			if (mario->GetBounds().Right() > GetPosition().X)
			{
				mario->SetState(new GoToCastleMarioState());
				touched = true;
			}
		}
		else if (GetPosition().Y < cell_y)
			Move(Vector::Down * delta_time * 0.25f);

		animator.Update(delta_time);
	}

	void EndLevelFlag::Init()
	{
		SetSize({ GetProperty("Width").AsFloat(), GetProperty("Height").AsFloat() });
		mario = GetParent()->FindObjectByName<Mario>("Mario");

		Blocks* blocks = GetParent()->FindObjectByType<Blocks>();
		cell_y = blocks->Height() - blocks->BlockSize().Y * 4;
	}
}