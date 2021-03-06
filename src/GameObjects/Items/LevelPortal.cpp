#include "LevelPortal.h"
#include "Mario.h"
#include "MarioGame.h"
#include "MarioState.h"
#include "GameScene.h"

using namespace Creatures;

namespace GameObjects
{
	LevelPortal::LevelPortal()
	{

	}

	void LevelPortal::GoToLevel()
	{
		sMarioGame->LoadLevel(level_name);
		if (show_status)
			sMarioGame->ShowStatus();
	}

	void LevelPortal::GoToSubLevel()
	{
		if (return_portal)
		{
			mario->SetUnclimb();
			mario->SetPosition(return_portal->GetBounds().Center().X - mario->GetBounds().Width / 2.f,
				               return_portal->GetBounds().Bottom() - mario->GetBounds().Height);

			if (return_portal->direction != Vector::Zero)
			{
				mario->Move(-return_portal->direction * TRANSITION_TIME * 0.03f);
				if (mario->GetRank() != MarioRank::Small)
					mario->Move(-return_portal->direction * 32.f);
				mario->SetState(new TransitionMarioState(return_portal->direction * 0.03f, TRANSITION_TIME));
			}

			return_portal->used = true;
			GetParent()->CastTo<GameScene>()->SetCameraOnTarget();
		}

		sMarioGame->LoadSubLevel(sub_level_name);
	}

	void LevelPortal::ReturnFromSubLevel()
	{
		sMarioGame->UnloadSubLevel();
	}

	void LevelPortal::Update(int delta_time)
	{
		bool contain_mario = GetBounds().IsContain(mario->GetBounds());
		bool is_valid_direction = direction == Vector::Zero || mario->GetInputDirection() == direction;
		bool is_valid_mario = mario->IsGrounded() || direction == Vector::Zero || mario->IsClimbing();

		if (!used && contain_mario && is_valid_direction && is_valid_mario)
		{
			used = true;

			if (direction != Vector::Zero)
				mario->SetState(new TransitionMarioState(direction * 0.03f, TRANSITION_TIME));

			switch (portal_type)
			{
				case PortalType::EnterLevel:
				{
					sMarioGame->GetTimer().Invoke(std::bind(&LevelPortal::GoToLevel, this), TRANSITION_TIME);

					break;
				}
				case PortalType::EnterSubLevel:
				{
					sMarioGame->GetTimer().Invoke(std::bind(&LevelPortal::GoToSubLevel, this), (direction != Vector::Zero) ? TRANSITION_TIME : 0);

					break;
				}
				case PortalType::ReturnSubLevel:
				{
					sMarioGame->GetTimer().Invoke(std::bind(&LevelPortal::ReturnFromSubLevel, this), (direction != Vector::Zero) ? TRANSITION_TIME : 0);
					break;
				}
				default:
					break;
			}
		}
	}

	void LevelPortal::Init()
	{
		SetSize({ GetProperty("Width").AsFloat(), GetProperty("Height").AsFloat() });
		mario = GetParent()->FindObjectByName<Creatures::Mario>("Mario");
		direction = ToVector(GetProperty("Direction").AsString());

		if (GetProperty("Level").IsValid() && !GetProperty("Level").AsString().empty())
		{
			level_name = GetProperty("Level").AsString();
			portal_type = PortalType::EnterLevel;
		}
		else if (GetProperty("SubLevel").IsValid() && !GetProperty("SubLevel").AsString().empty())
		{
			sub_level_name = GetProperty("SubLevel").AsString();
			portal_type = PortalType::EnterSubLevel;

			if (GetProperty("ComebackPortal").IsValid() && !GetProperty("ComebackPortal").AsString().empty())
			{
				std::string return_portal_str = GetProperty("ComebackPortal").AsString();
				return_portal = GetParent()->FindObjectByName<LevelPortal>(return_portal_str);
				assert(return_portal);
			}
		}
		portal_type = PortalType::ReturnSubLevel;

		if (GetProperty("ShowStatus").IsValid())
			show_status = GetProperty("ShowStatus").AsBool();
	}
}