#pragma once
#include "Graphics.h"
#include "Location.h"

class Board {
public:
	Board(Graphics &gfx);

	static constexpr int llw = 1;
	static constexpr int llh = 1;
	void drawcell(const Location & loc, Color c);
	void drawcellthick(const Location &loc, Color c, int thick);
	int Getwidth() const { return width; }
	int Getheight()const { return height; }
	void DrawBorder();
	bool IsInsideBoard(const Location& loc) const;
public:
	static constexpr int dimension = 20;
	Graphics &gfx;
	static constexpr int width = (Graphics::ScreenWidth / dimension) -llw;
	static constexpr int height = (Graphics::ScreenHeight / dimension) -llh;
};