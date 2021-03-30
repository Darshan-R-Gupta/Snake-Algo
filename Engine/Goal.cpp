#include "Goal.h"

Goal::Goal(std::mt19937 & rng, const Board & brd, Snake & snake,Graphics &gfx)
	:gfx(gfx)
{
	Respawn(rng, brd, snake);
}

void Goal::Respawn(std::mt19937 & rng, const Board & brd, Snake & snake)
{
	std::uniform_int_distribution<int> xd(brd.llw, brd.Getwidth()  -1);
	std::uniform_int_distribution<int> yd(brd.llh, brd.Getheight() -1);
	Location newloc;
	do {
		newloc.x = xd(rng);
		newloc.y = yd(rng);
	} while (snake.IsInTile(newloc));
	loc = newloc;

}

void Goal::Draw(Board & brd) const {
	brd.drawcellthick(loc, Colors::Cyan, (int) 10 );
}

const Location & Goal::getloc() const
{
	return loc;
}
