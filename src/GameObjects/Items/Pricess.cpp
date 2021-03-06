#include "Pricess.h"
#include "MarioGame.h"

using namespace GameEngine;

namespace GameObjects
{
	Princess::Princess()
	{
		SetSize({ 32, 64 });
		animator.Create("stay", *sMarioGame->TextureManager().Get("Items"), { 222, 96, 32, 64 });
	}

	void Princess::Draw(sf::RenderWindow* render_window)
	{
		animator.SetPosition(GetPosition());
		animator.Draw(render_window);
	}
}