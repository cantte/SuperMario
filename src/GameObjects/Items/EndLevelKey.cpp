#include "EndLevelKey.h"
#include "Mario.h"
#include "MarioGame.h"
#include "Bowser.h"
#include "Blocks.h"
#include "MarioState.h"
#include "GameScene.h"
#include "Text.h"
#include "LevelPortal.h"

namespace GameObjects
{
	EndLevelKey::EndLevelKey()
	{
		sprite.setTexture(*sMarioGame->TextureManager().Get("Items"));
		sprite.setTextureRect({ 0, 212, 32, 32 });
		SetSize({ 32, 32 });
	}

	void EndLevelKey::Draw(sf::RenderWindow* render_window)
	{
		sprite.setPosition(GetPosition());
		render_window->draw(sprite);
	}

	void EndLevelKey::Update(int delta_time)
	{
		switch (state)
		{
			case State::Play:
			{
				if (mario->GetBounds().IsIntersect(GetBounds()))
				{
					if (GetParent()->FindObjectByName("Bowser"))
						SetState(State::BridgeHiding);
					else
					{
						sMarioGame->StopMusic();
						Hide();
						sMarioGame->PlaySound("world_clear");
						SetState(State::MarioGoToPrincess);
					}
				}

				break;
			}
			case State::BridgeHiding:
			{
				delay_timer -= delta_time;
				if (delay_timer < 0)
				{
					delay_timer = 100;
					Vector block = bridge_blocks.back();
					blocks->ClearBlock(block.X, block.Y);
					bridge_blocks.pop_back();

					if (bridge_blocks.empty())
						SetState(State::BowserRun);
					sMarioGame->PlaySound("breakblock");
				}

				break;
			}
			case State::BowserRun:
			{
				delay_timer -= delta_time;
				if (delay_timer < 0)
				{
					SetState(State::MarioGoToPrincess);
					sMarioGame->PlaySound("world_clear");
				}

				break;
			}
			case State::MarioGoToPrincess:
			{
				if (mario->GetSpeed().X == 0)
					SetState(State::TextShow);

				break;
			}
			case State::TextShow:
			{
				delay_timer -= delta_time;
				if (delay_timer < 0)
				{
					GameScene* gameScene = GetParent()->CastTo<GameScene>();
					Label* text = sMarioGame->CreateText("But our princess is in\n another castle!",
						gameScene->ScreenToPoint(sMarioGame->ScreenSize() / 2) - Vector(120, 40));
					GetParent()->AddObject(text);

					SetState(State::GotToNextLevel);
				}

				break;
			}
			case State::GotToNextLevel:
			{
				delay_timer -= delta_time;
				if (delay_timer < 0)
				{
					LevelPortal* portal = GetParent()->FindObjectByType<LevelPortal>();

					assert(portal->GetProperty("Level").IsValid());

					sMarioGame->LoadLevel(portal->GetProperty("Level").AsString());
					sMarioGame->ShowStatus();
				}

				break;
			}
			default:
				break;
		}
	}

	void EndLevelKey::SetState(State state)
	{
		this->state = state;

		switch (state)
		{
			case State::BridgeHiding:
			{
				mario->Disable();
				bowser->Disable();
				sMarioGame->StopMusic();
				Hide();

				bridge_blocks = blocks->GetBridgeBlocks();
				delay_timer = 500;

				break;
			}
			case State::BowserRun:
			{
				bowser->Enable();
				bowser->NoBridge();
				delay_timer = 1000;

				break;
			}
			case State::MarioGoToPrincess:
			{
				mario->TurnOn();
				mario->SetState(new Creatures::GoToPrincessState());

				break;
			}
			case State::TextShow:
			{
				GameScene* gameScene = GetParent()->CastTo<GameScene>();
				Label* text = sMarioGame->CreateText("Thank you Mario!", 
						gameScene->ScreenToPoint(sMarioGame->ScreenSize() / 2) - Vector(100, 100));
				GetParent()->AddObject(text);
				delay_timer = 1000;

				break;
			}
			case State::GotToNextLevel:
			{
				delay_timer = 5000;
				break;
			}
			default:
				break;
		}
	}

	void EndLevelKey::Init()
	{
		mario = GetParent()->FindObjectByType<Creatures::Mario>();
		bowser = GetParent()->FindObjectByType<Creatures::Bowser>();
		blocks = GetParent()->FindObjectByType<Blocks>();
	}
}