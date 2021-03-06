#include "PlatformSytem.h"
#include "Platform.h"
#include "MarioGame.h"

namespace GameObjects
{
	PlatformSystem::PlatformSystem()
	{
		sf::Texture& textute = *sMarioGame->TextureManager().Get("Items");

		for (auto& sprite : sprites)
			sprite.setTexture(textute);

		sprites[0].setTextureRect({ 100 ,16,32,32 });
		sprites[1].setTextureRect({ 132 ,16,-32,32 });
		sprites[2].setTextureRect({ 132,16,32,32 }); //h
		sprites[3].setTextureRect({ 100,48,32,32 });
		sprites[4].setTextureRect({ 132,48,-32,32 });
	}

	void PlatformSystem::Draw(sf::RenderWindow* render_window)
	{
		GameObject::Draw(render_window);

		int width = GetBounds().Width / 6 - 16;
		sprites[0].setPosition(GetPosition() + Vector::Right * width);
		render_window->draw(sprites[0]);
		sprites[1].setPosition(GetBounds().RightTop() + Vector::Left * 32 + Vector::Left * width);
		render_window->draw(sprites[1]);

		int left_shoulder = left_platform->GetPosition().Y - GetPosition().Y - 32;
		GameEngine::DrawLinearSprite_v(sprites[3], { (int)GetPosition().X + width, (int)GetPosition().Y + 32, 32, left_shoulder }, render_window);

		int right_shoulder = right_platform->GetPosition().Y - GetPosition().Y - 32;
		GameEngine::DrawLinearSprite_v(sprites[4], { (int)GetBounds().Right() - (width + 32), (int)GetPosition().Y + 32, 32, right_shoulder }, render_window);
		GameEngine::DrawLinearSprite_h(sprites[2], { (int)GetPosition().X + 32 + width, (int)GetPosition().X, (int)GetBounds().Width - 2 * (32 + width), 32 }, render_window);
	}

	void PlatformSystem::Update(int delta_time)
	{
		GameObject::Update(delta_time);
	}

	void PlatformSystem::OnActivated()
	{
		SetSize({ GetProperty("Width").AsFloat(), GetProperty("Height").AsFloat() });

		left_platform = new FallingPlatform();
		right_platform = new FallingPlatform();

		float width = GetBounds().Width / 3;
		left_platform->SetProperty("Width", Property(width));
		right_platform->SetProperty("Width", Property(width));
		left_platform->SetPosition(GetBounds().LeftButton() - Vector::Up * 16 + Vector::Right * width / 2);
		right_platform->SetPosition(GetBounds().LeftButton() - Vector::Up * 16 + Vector::Right * 2 * width + Vector::Right * width / 2);

		AddObject(left_platform);
		AddObject(right_platform);

		left_platform->SetMovingCallback(std::bind(&PlatformSystem::OnLeftPlatformMove, this));
		right_platform->SetMovingCallback(std::bind(&PlatformSystem::OnRightPlatformMove, this));
	}

	void PlatformSystem::OnLeftPlatformMove()
	{
		if (left_platform->GetPosition().Y - GetPosition().Y > 2 * GetBounds().Height - 32)
			left_platform->SetSpeed(Vector::Zero);
		else
		{
			float wr = 2 * (GetBounds().Height + GetPosition().Y) - left_platform->GetPosition().Y;
			right_platform->SetPosition(right_platform->GetPosition().X, wr);
		}
	}

	void PlatformSystem::OnRightPlatformMove()
	{
		if (right_platform->GetPosition().Y - GetPosition().Y > 2 * GetBounds().Height - 32)
			right_platform->SetSpeed(Vector::Zero);
		else
		{
			float wl = 2 * (GetBounds().Height + GetPosition().Y) - right_platform->GetPosition().Y;
			left_platform->SetPosition(right_platform->GetPosition().X, wl);
		}
	}
}