#include "Background.h"
#include "GameScene.h"
#include "MarioGame.h"
#include "Blocks.h"

using namespace GameEngine;

namespace GameObjects
{
	Background::Background()
	{
		background.setTextureRect({ 0, 0, 1280, 720 });
	}

	void Background::Draw(sf::RenderWindow* render_window)
	{
		Rect cameraRect = GetParent()->CastTo<GameEngine::GameScene>()->CameraRect();
		background.setPosition(cameraRect.LeftTop());
		render_window->draw(background);
	}

	void Background::Init()
	{
		if (GetProperty("NightViewFilter").IsValid() && GetProperty("NightViewFilter").AsBool())
			GetParent()->FindObjectByType<Blocks>()->EnableNightViewFilter();
	}

	void Background::OnActivated()
	{
		background.setTexture(*sMarioGame->TextureManager().Get(GetProperty("Picture").AsString()));
		MoveToBack();
	}
}