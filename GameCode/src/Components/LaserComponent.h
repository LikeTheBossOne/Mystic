#pragma once
#include "Mystic/Core/MMacros.h"
#include "Mystic/Scripting/NativeScriptComponent.h"

MSTRUCT()
class LaserComponent : Mystic::NativeScriptComponent
{
	//MGENERATED_BODY()
	// This will expand to
	// public:
	//   LaserComponent(Mystic::Entity* e)
	//   }
	//       _owningEntity = e;
	//   }
public:
	LaserComponent(entt::entity e, Mystic::Scene* scene)
	{
		_ent = e;
		_owningScene = scene;
	}



public:
	MPROPERTY()
	float Damage = 0.0f;

	void Start();
	void Update();

	LaserComponent(const LaserComponent&) = default;
	
};