#ifndef TRIGGER_H
#define TRIGGER_H

#include "GameObject.h"

namespace Creatures
{
	class Mario;
}

namespace GameObjects
{
	class Trigger : public GameObject
	{
	private:
		bool trigered = false;
		Creatures::Mario* mario = nullptr;

		void Update(int delta_time) override;
		void Init() override;
		void OnActivated() override;
	};
}

#endif // !TRIGGER_H
