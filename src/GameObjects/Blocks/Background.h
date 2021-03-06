#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "GameObject.h"

namespace GameObjects
{
	class Background : public GameObject
	{
	public:
		Background();

		virtual void Draw(sf::RenderWindow* render_window);

	private:
		sf::Sprite background;

		void Init() override;
		void OnActivated() override;
	};
}

#endif // !BACKGROUND_H
