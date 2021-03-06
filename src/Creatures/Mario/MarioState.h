#ifndef MARIO_STATE_H
#define MARIO_STATE_H

#include "Geometry.h"

#include <string>

namespace GameObjects
{
	class Princess;
}

namespace Creatures
{
	using namespace GameEngine::Geometry;

	enum class MarioState
	{
		Normal,
		Promoting,
		Demoting,
		Died
	};

	enum class MarioRank
	{
		Small,
		Big,
		Fire
	};

	class Mario;

	class IMarioState
	{
	public:
		~IMarioState() = default;

		virtual void OnEnter() = 0;
		virtual void OnLeave() = 0;
		virtual void Update(int delta_time) = 0;

		void SetMario(Mario* mario) { this->mario = mario; }
	protected:
		Mario* GetMario() { return mario; }

		void SetMarioSpawnProtection();
		void PlayAnimation(const std::string& anim_name);
		void SetMarioRank(const MarioRank& rank);
		void SetMarioState(const MarioState& state);
		void SetMarioNormalPallete();
		void SetMarioFirePallete();
		void EnableScene(bool value);

	private:
		Mario* mario = nullptr;
	};

	class NormalMarioState : public IMarioState
	{
	public:
		void OnEnter() override { }
		void OnLeave() override { }
		void Update(int delta_time) override;
	};

	class DiedMarioState : public IMarioState
	{
	public:
		void OnEnter() override;
		void OnLeave() override { }
		void Update(int delta_time) override;

	private:
		int delay_timer = 3000;
	};

	class PromotingMarioState : public IMarioState
	{
	public:
		void OnEnter() override;
		void OnLeave() override;
		void Update(int delta_time) override;

	private:
		int promoting_timer = 0;
	};

	class DemotingMarioState : public IMarioState
	{
	public:
		void OnEnter() override;
		void OnLeave() override;
		void Update(int delta_time) override;

	private:
		int promoting_timer = 0;
	};

	class TransitionMarioState : public IMarioState
	{
	public:
		TransitionMarioState(const Vector& speed, int time);

		void OnEnter() override;
		void OnLeave() override;
		void Update(int delta_time) override;

	private:
		Vector speed;
		int timer = 0;
		int time;
	};

	class GoToCastleMarioState : public IMarioState
	{
	public:
		GoToCastleMarioState() { }

		void OnEnter() override;
		void OnLeave() override { }
		void Update(int delta_time) override;

	private:
		enum class State
		{
			Start,
			GoDown,
			Overturn,
			Walk,
			Wait,
			NextLevel
		}state = State::Start;

		Vector speed;
		int timer = 0;
		int cell_y = 0;
		int delay_timer = 0;
		std::string next_level;
		std::string next_sub_level;
	};

	class GoToPortalState : public IMarioState
	{
	public:
		void OnEnter() override;
		void OnLeave() override { }
		void Update(int delta_time) override;
	};

	class GoToPrincessState : public IMarioState
	{
	public:
		GoToPrincessState() { }

		void OnEnter() override;
		void OnLeave() override { }
		void Update(int delta_time) override;

	private:
		GameObjects::Princess* princess = nullptr;
	};
}
#endif // !MARIO_STATE_H
