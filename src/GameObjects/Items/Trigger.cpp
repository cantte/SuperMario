#include "Trigger.h"
#include "Mario.h"
#include "GameEngine.h"

namespace GameObjects
{
	void Trigger::Update(int delta_time)
	{
		if (!trigered && GetBounds().IsContain(mario->GetBounds()))
		{
			if (GetProperty("EnableAction").IsValid())
			{
				auto object_names = Split(GetProperty("EnableAction").AsString(), ';');
				for (auto object_name : object_names)
				{
					if (GameObjects::GameObject* object = GetParent()->FindObjectByName(object_name))
						object->Show();
				}
			}

			trigered = true;
		}
	}

	void Trigger::Init()
	{
		mario = GetParent()->FindObjectByName<Creatures::Mario>("Mario");
	}

	void Trigger::OnActivated()
	{
		SetSize({ GetProperty("Width").AsFloat(), GetProperty("Height").AsFloat() });
	}
}