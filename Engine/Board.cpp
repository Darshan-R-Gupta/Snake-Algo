#include "Board.h"
#include <assert.h>
Board::Board(Graphics & gfx)
	: gfx(gfx)
{

}

void Board::drawcell(const Location & loc, Color c)
{
	assert(loc.x >= llw);
	assert(loc.x < Getwidth());
	assert(loc.y >= llh);
	assert(loc.y < Getheight());
	gfx.DrawRectDim( loc.x * dimension, loc.y*dimension, dimension, dimension, c);
}

void Board::drawcellthick(const Location & loc, Color c, int thick)
{
	assert(loc.x >= llw);
	assert(loc.x < Getwidth());
	assert(loc.y >= llh);
	assert(loc.y < Getheight());
	gfx.DrawRectDim(loc.x * dimension, loc.y*dimension, dimension - thick, dimension - thick, c);

}

void Board::DrawBorder()
{
	for (int i = llw *dimension; i <= width *dimension; i++) {
		gfx.PutPixel(i, llh*dimension, Colors::Green);
	}
	for (int j = llh *dimension; j <= height *dimension; j++) {
		gfx.PutPixel(llw*dimension,j, Colors::Green);
	}
	for (int k = llh * dimension; k <= height*dimension; k++) {
		gfx.PutPixel(width*dimension,k , Colors::Green);
	}
	for (int l = llw * dimension; l <= width*dimension; l++) {
		gfx.PutPixel(l, height*dimension, Colors::Green);
	}
	
}

bool Board::IsInsideBoard(const Location & loc) const
{
	return loc.x >= llw  && loc.x < width &&
		   loc.y >= llh && loc.y < height;
}
