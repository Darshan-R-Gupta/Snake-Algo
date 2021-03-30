#pragma once
#include "Snake.h"
#include "Board.h"
#include <random>
#include "Graphics.h"
class Goal {
public:
	Goal(std::mt19937& rng, const Board & brd, Snake& snake,Graphics &gfx);
	void Respawn(std::mt19937& rng, const Board & brd,Snake& snake);
	void Draw(Board& brd) const;
	const Location& getloc() const;
	Graphics &gfx;

	Location loc;

	Color c = Colors::Green;
private: 
};