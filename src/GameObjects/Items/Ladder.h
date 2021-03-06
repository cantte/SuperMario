#ifndef LADDER_H
#define LADDER_H

#include "GameObject.h"

namespace Creatures
{
	class Mario;
}

namespace GameObjects
{
	class Ladder : public GameObject
	{
	public:
		Ladder(const Vector& pos, Creatures::Mario* mario);

		void Draw(sf::RenderWindow* render_window) override;
		void Update(int delta_time) override;

		virtual void OnActivated() override;
		virtual void Init() override;

	private:
		sf::Sprite sprite;
		float height, width, bottom;
		float timer = 0;
	};
}

#endif // !LADDER_H
