#ifndef LEVEL_PORTAL_H
#define LEVEL_PORTAL_H

#include "GameObject.h"

namespace Creatures
{
	class Mario;
}

namespace GameObjects
{
	class LevelPortal : public GameObject
	{
	public:
		LevelPortal();

	private:
		Creatures::Mario* mario;
		bool used = false;
		bool show_status = false;
		LevelPortal* return_portal = nullptr;
		std::string level_name;
		std::string sub_level_name;

		const int TRANSITION_TIME = 1500;
		float timer = 0;

		enum class PortalType
		{
			EnterLevel,
			EnterSubLevel,
			ReturnSubLevel
		} portal_type;

		enum class State
		{
			Wait,
			Transition
		} state = State::Wait;

		Vector direction;

		void GoToLevel();
		void GoToSubLevel();
		void ReturnFromSubLevel();

		virtual void Update(int delta_time) override;
		virtual void Init() override;
	};
}

#endif // !LEVEL_PORTAL_H
