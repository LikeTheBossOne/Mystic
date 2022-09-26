#pragma once

struct PlayerComponent
{
	bool Alive;
	float TimeSinceShotLast;

	PlayerComponent() : Alive(true), TimeSinceShotLast(5) {}
};