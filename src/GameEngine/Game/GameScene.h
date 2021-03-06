#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include "GameObject.h"

namespace Creatures
{
	class Mario;
}

namespace GameObjects
{
	class Blocks;
}

namespace GameEngine
{
	class GameScene : public GameObjects::GameObject
	{
	public:
		GameScene(const std::string& filepath);
		~GameScene();

		const std::string& GetLevelName() const;

		void LoadFromFile(const std::string& filepath);

		Rect CameraRect() const;
		void SetCameraOnTarget();
		Vector PointToScreen(const Vector& vector);
		Vector ScreenToPoint(const Vector& vector);

		void PlaySoundAtPoint(const std::string& name, const Vector& pos);

	protected:
		virtual void Update(int delta_time) override;
		virtual void Draw(sf::RenderWindow* render_window) override;
		virtual void Events(const sf::Event& event) override;

	private:
		Creatures::Mario* mario = nullptr;
		GameObjects::Blocks* blocks = nullptr;
		sf::View view;
		std::string level_name;

		const float scale_factor = 1.5f;
		const Vector screen_size = Vector(1280 / scale_factor, 720 / scale_factor);

		GameScene();

		void Init();
	};
}
#endif // !GAME_SCENE_H