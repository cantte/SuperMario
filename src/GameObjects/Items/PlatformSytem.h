#ifndef PLATFORM_SYSTEM_H
#define PLATFORM_SYSTEM_H

#include "GameObject.h"

namespace GameObjects
{
	class FallingPlatform;

	class PlatformSystem : public GameObject
	{
	public:
		PlatformSystem();

		void Draw(sf::RenderWindow* render_window) override;
		void Update(int delta_time) override;
		void OnActivated() override;

	private:
		FallingPlatform* left_platform = nullptr;
		FallingPlatform* right_platform = nullptr;
		sf::Sprite sprites[5];

		void OnLeftPlatformMove();
		void OnRightPlatformMove();
	};
}

#endif // !PLATFORM_SYSTEM_H
