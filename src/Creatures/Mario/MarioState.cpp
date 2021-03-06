#include "MarioState.h"
#include "CastleFlag.h"
#include "Mario.h"
#include "MarioGame.h"
#include "GameScene.h"
#include "PiranhaPlant.h"
#include "Pricess.h"
#include "LevelPortal.h"
#include "Blocks.h"

using namespace GameObjects;

namespace Creatures
{
	void IMarioState::SetMarioSpawnProtection()
	{
		GetMario()->spawn_timer = 3000;
	}

	void IMarioState::PlayAnimation(const std::string& anim_name)
	{
		GetMario()->PlayAnimation(anim_name);
	}

	void IMarioState::SetMarioRank(const MarioRank& rank)
	{
		GetMario()->SetRank(rank);
	}

	void IMarioState::SetMarioState(const MarioState& state)
	{
		GetMario()->SetState(state);
	}

	void IMarioState::SetMarioNormalPallete()
	{
		GetMario()->animator->SetPallete(nullptr);
	}

	void IMarioState::SetMarioFirePallete()
	{
		GetMario()->animator->SetPallete(&(GetMario()->fire_pallete));
	}

	void IMarioState::EnableScene(bool value)
	{
		auto mario = GetMario();

		GetMario()->GetParent()->ForeachObject(
			[mario, value](GameObjects::GameObject* obj)
			{
				if (obj != mario)
					if (value)
						obj->Enable();
					else
						obj->Disable();
			}
		);
	}

	void NormalMarioState::Update(int delta_time)
	{
		auto mario = GetMario();
		mario->InputProcessing(delta_time);
		mario->PhysicsProccesing(delta_time);
		mario->CollisionProcessing(delta_time);
		mario->KickBlocksProcessing();
		mario->KickEnemiesProcessing(delta_time);
		mario->AnimationProcessing(delta_time);

		static int screed_height = sMarioGame->ScreenSize().Y - 200;
		if (mario->GetBounds().Bottom() > screed_height)
		{
			if (mario->GetParent()->CastTo<GameEngine::GameScene>()->
				GetLevelName().substr(0, 2) != "sk")
				SetMarioState(MarioState::Died);

			if (mario->invicible_mode)
			{
				sMarioGame->InvicibleMode(false);
				sMarioGame->StopMusic();
			}
		}

		if (mario->invicible_mode)
		{
			mario->invincible_timer -= delta_time;

			Pallete* pallete = (int(mario->invincible_timer / 100) % 2) ? &(mario->black_pallete) : &(mario->fire_pallete);
			mario->animator->SetPallete(pallete);

			if (mario->invincible_timer < 0)
			{
				Pallete* other_pallete = (mario->GetRank() == MarioRank::Fire) ? &(mario->fire_pallete) : nullptr;
				mario->animator->SetPallete(other_pallete);
				mario->invicible_mode = false;
				sMarioGame->InvicibleMode(false);
			}
		}

	}

	void DiedMarioState::OnEnter()
	{
		EnableScene(false);

		GetMario()->SetRank(MarioRank::Small);
		GetMario()->animator->Play("died");
		GetMario()->speed = Vector::Zero;
		GetMario()->AddImpulse(Vector::Up * 0.8f);
		GetMario()->SetGrounded(false);
		GetMario()->jumped = false;

		sMarioGame->MusicManager().Stop();
		sMarioGame->PlaySound("mario_die");
	}

	void DiedMarioState::Update(int delta_time)
	{
		GetMario()->PhysicsProccesing(delta_time);
		delay_timer -= delta_time;

		if (delay_timer < 0)
			sMarioGame->MarioDied();
	}

	void PromotingMarioState::OnEnter()
	{
		promoting_timer = 1500;

		GetMario()->animator->SetColor(sf::Color::White);
		EnableScene(false);

		if (GetMario()->GetRank() == MarioRank::Small)
		{
			SetMarioRank(MarioRank::Big);
			PlayAnimation("growing");
		}
		else if (GetMario()->GetRank() == MarioRank::Big)
			SetMarioRank(MarioRank::Fire);

		GetMario()->Show();
	}

	void PromotingMarioState::OnLeave()
	{
		EnableScene(true);

		if (GetMario()->GetRank() == MarioRank::Fire)
			SetMarioFirePallete();
	}

	void PromotingMarioState::Update(int delta_time)
	{
		if (GetMario()->GetRank() == MarioRank::Fire)
		{
			Pallete* pallete = (int(promoting_timer / 100) % 2) ? &(GetMario()->fire_pallete) : nullptr;
			GetMario()->animator->SetPallete(pallete);
		}

		promoting_timer -= delta_time;
		if (promoting_timer < 0)
			SetMarioState(MarioState::Normal);
	}

	void DemotingMarioState::OnEnter()
	{
		EnableScene(false);
		promoting_timer = 1500;

		SetMarioNormalPallete();
		SetMarioRank(MarioRank::Small);
		PlayAnimation("demoting");
		sMarioGame->PlaySound("pipe");

		GetMario()->Show();
	}

	void DemotingMarioState::OnLeave()
	{
		EnableScene(true);
		SetMarioSpawnProtection();
	}

	void DemotingMarioState::Update(int delta_time)
	{
		PlayAnimation("demoting");
		promoting_timer -= delta_time;
		if (promoting_timer < 0)
			SetMarioState(MarioState::Normal);
	}

	TransitionMarioState::TransitionMarioState(const Vector& speed, int time)
	{
		this->speed = speed;
		timer = time;
		this->time = 0;
	}

	void TransitionMarioState::OnEnter()
	{
		GetMario()->SetSpeed(speed.Normalized() * GetMario()->walk_speed / 2.f);
		GetMario()->MoveUnderTo(GetMario()->GetParent()->FindObjectByType<Blocks>());

		auto piranas_plant = GetMario()->GetParent()->FindObjectsByType<PiranhaPlant>();
		for (auto& pirana : piranas_plant)
			pirana->HideInTube();

		if (speed.Y > 0 || speed.X > 0)
			sMarioGame->PlaySound("pipe");

	}

	void TransitionMarioState::OnLeave()
	{
		GetMario()->MoveToFront();
		GetMario()->SetSpeed(Vector::Zero);
	}

	void TransitionMarioState::Update(int delta_time)
	{
		if (timer > 0)
		{
			timer -= delta_time;
			GetMario()->Move(delta_time * speed);
			GetMario()->AnimationProcessing(delta_time);
		}
		else
			SetMarioState(MarioState::Normal);
	}

	void GoToCastleMarioState::OnEnter()
	{
		sMarioGame->MusicManager().Stop();

		auto block = GetMario()->GetParent()->FindObjectByType<GameObjects::Blocks>();
		cell_y = block->Height() - block->BlockSize().Y * 4 - (GetMario()->IsSmall() ? 2 : 34);
		GetMario()->input_direction = Vector::Zero;

		if (GetMario()->IsSeated())
			GetMario()->StandUp();
	}

	void GoToCastleMarioState::Update(int delta_time)
	{
		switch (state)
		{
			case State::Start:
			{
				GetMario()->animator->FlipX(false);

				auto anim_name = std::string("climb_") + (GetMario()->IsSmall() ? "small" : "big");
				PlayAnimation(anim_name);
				GetMario()->animator->SetSpeed(anim_name, 0.005f);
				state = State::GoDown;
				sMarioGame->PlaySound("flagpole");
				break;
			}
			case State::GoDown:
			{
				if (GetMario()->GetPosition().Y < cell_y)
					GetMario()->Move(Vector::Down * delta_time * 0.25f);
				else
				{
					state = State::Overturn;
					GetMario()->animator->FlipX(true);
					GetMario()->Move(Vector::Right * 32.f);
					delay_timer = 500;
				}
				break;
			}
			case State::Overturn:	
			{
				delay_timer -= delta_time;

				if (delay_timer < 0)
				{
					state = State::Walk;

					sMarioGame->PlaySound("stage_clear");
					auto anim_name = std::string("walk_") + (GetMario()->IsSmall() ? "small" : "big");
					PlayAnimation(anim_name);
					GetMario()->animator->SetSpeed(anim_name, 0.003f);
					GetMario()->animator->FlipX(false);
					GetMario()->input_direction = Vector::Zero;
				}
				break;
			}
			case State::Walk:
			{
				GetMario()->AddImpulse(Vector::Right * 10.f);
				GetMario()->PhysicsProccesing(delta_time);
				GetMario()->CollisionProcessing(delta_time);

				auto portals = GetMario()->GetParent()->FindObjectsByType<GameObjects::LevelPortal>();
				for (auto& portal : portals)
				{
					if (portal->GetBounds().IsContainByX(GetMario()->GetPosition()))
					{
						state = State::Wait;
						GetMario()->Hide();

						next_level = portal->GetProperty("Level").AsString();
						if (portal->GetProperty("SubLevel").IsValid())
							next_sub_level = portal->GetProperty("SubLevel").AsString();

						sMarioGame->SetEndLevelStatus();
					}
				}

				break;
			}
			case State::Wait:
			{
				if (sMarioGame->GetGameTime() == 0)
				{
					delay_timer = 3500;
					state = State::NextLevel;

					auto flag = GetMario()->GetParent()->FindObjectByType<GameObjects::CastleFlag>();
					assert(flag);
					flag->LiftUp();
				}
				break;
			}
			case State::NextLevel:
			{
				delay_timer -= delta_time;

				if (delay_timer < 0)
				{
					sMarioGame->LoadLevel(next_level);

					if (!next_sub_level.empty())
						sMarioGame->LoadSubLevel(next_sub_level);
					sMarioGame->ShowStatus();
				}
				break;
			}
			default:
				break;
		}

		GetMario()->animator->Update(delta_time);
	}

	void GoToPortalState::OnEnter()
	{
		auto anim_name = std::string("walk_") + (GetMario()->IsSmall() ? "small" : "big");
		PlayAnimation(anim_name);
		GetMario()->animator->SetSpeed(anim_name, 0.003f);
	}

	void GoToPortalState::Update(int delta_time)
	{
		GetMario()->input_direction = Vector::Right;
		GetMario()->speed.X = GetMario()->walk_speed * 0.6f;
		GetMario()->Move(GetMario()->GetSpeed() * delta_time);

		GetMario()->CollisionProcessing(delta_time);
		GetMario()->collision_tag |= CollisionTag::Right;
		GetMario()->SetGrounded();
	}

	void GoToPrincessState::OnEnter()
	{
		princess = GetMario()->GetParent()->FindObjectByType<Princess>();

		GetMario()->input_direction = Vector::Right;
		GetMario()->direction = Vector::Right;

		auto anim_name = std::string("walk_") + (GetMario()->IsSmall() ? "small" : "big");
		PlayAnimation(anim_name);
		GetMario()->animator->SetSpeed(anim_name, 0.003f);
		GetMario()->animator->FlipX(false);

		if (GetMario()->IsSeated())
			GetMario()->StandUp();
	}

	void GoToPrincessState::Update(int delta_time)
	{
		if (princess->GetPosition().X - GetMario()->GetBounds().Right() > 16)
		{
			GetMario()->speed.X = GetMario()->walk_speed * 0.6f;
			GetMario()->PhysicsProccesing(delta_time);
			GetMario()->CollisionProcessing(delta_time);

			if (GetMario()->IsGrounded())
				GetMario()->animator->Update(delta_time);
		}
		else
		{
			GetMario()->PlayAnimation("idle_" + std::string(GetMario()->IsSmall() ? "small" : "big"));
			GetMario()->speed.X = 0;
		}
	}
}