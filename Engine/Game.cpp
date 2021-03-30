/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"
#include "SpriteCodex.h"
#include <iostream>

#define	VK_W 87
#define VK_S 83
#define VK_A 65
#define VK_D 68
int ai_type = 1;
Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	rng(std::random_device()()),
	brd( gfx ),
	snake({ brd.Getwidth()/2, brd.Getheight()/2 }),
	goal(rng,brd,snake,gfx)
{
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
		//AI_move();
	               AI_move();
	if (!gameisover) {
		
		++snakecounter;

		if (snakecounter >= rate) {
			snakecounter = 0;
			const Location next = snake.getnextloc(del_loc);
			if (!brd.IsInsideBoard(next) || snake.IsInTileExceptend(next)) {
				gameisover = true;
			}
			else {
				bool eating = (next == goal.getloc());
				if (eating) {
					snake.Grow();
				}
				if (wnd.kbd.KeyIsPressed(VK_RETURN)) {
					del_loc = { 0,0 };
				}
				snake.Move(del_loc);
				if (eating) {
					goal.Respawn(rng, brd, snake);
				}
			};

		}
	}
}

void Game::reset()
{
	snake.resetsnake(brd);
}

bool isInsideBox( Location loc, Location corner[] ) {
	int maxx = corner[0].x;
	int minx = corner[0].x;
	for (int i = 0; i < 4; ++i) {
		if (maxx < corner[i].x)	maxx = corner[i].x;
		if (minx > corner[i].x)	minx = corner[i].x;
	}
	int maxy = corner[0].y;
	int miny = corner[0].y;
	for (int i = 0; i < 4; ++i) {
		if (maxy < corner[i].y)	maxx = corner[i].y;
		if (miny > corner[i].y)	minx = corner[i].y;
	}

	if (loc.x >= minx && loc.x <= maxx) {
		if (loc.y >= miny && loc.y <= maxy)	return true;
		else
		{
			return false;
		}
	}
	return false;
}
bool Game::is_blockage(int seg_id, Location corner[]) {
	if (seg_id == 0 || seg_id == snake.nseg - 1) {
		//	return false;
	}

	int maxx = corner[0].x;
	int minx = corner[0].x;
	for (int i = 0; i < 4; ++i) {
		if (maxx < corner[i].x)	maxx = corner[i].x;
		if (minx > corner[i].x)	minx = corner[i].x;
	}
	int maxy = corner[0].y;
	int miny = corner[0].y;
	for (int i = 0; i < 4; ++i) {
		if (maxy < corner[i].y)	maxx = corner[i].y;
		if (miny > corner[i].y)	minx = corner[i].y;
	}
	if (maxx - minx == 0 || maxy - miny == 0) {
		//single row;
		if (maxx - minx == 0) {
			for (int i = miny; i <= maxy; ++i) {
				if (snake.IsInTileExceptend({minx,i })) {
					return true;
				}
			}
		}
		else {
			for (int i = minx; i <= maxx; ++i) {
				if (snake.IsInTileExceptend({ i,miny })) {
					return true;
				}
			}
		}
	}
	bool min_reachedx = false;
	bool max_reachedx = false;

	bool min_reachedy = false;
	bool max_reachedy = false;
	for (int i = 1; i < snake.nseg - 1; ++i) {
		if (isInsideBox(snake.seg[i].loc, corner)) {
			if (snake.seg[i].loc.x == minx) {
				min_reachedx = true;
			}
			if (snake.seg[i].loc.x == maxx) {
				max_reachedx = true;
			}

			if (snake.seg[i].loc.y == miny) {
				min_reachedy = true;
			}
			if (snake.seg[i].loc.y == maxy) {
				max_reachedy = true;
			}

		}
	}

	int snake_corner = 0;
	int goal_corner = 0;
	for (int i = 0; i < 4; ++i) {
		if (snake.seg[0].loc == corner[i])	snake_corner = i;
		if (goal.getloc() == corner[i])		goal_corner = i;
	}

	bool verdict = (min_reachedx && max_reachedx) || (min_reachedy && max_reachedy);
	return verdict;
}

int Game::is_future_colliding(Location del_loc, Goal goal){
	Location next = snake.getnextloc(del_loc);
	if (next == goal.getloc())	return false;	//1 cell
	else {
		Location g = goal.getloc();
		Location curr = next;
		
		
		
		Location corner[4];
		corner[0] = { curr.x, g.y };
		corner[1] = { g.x, curr.y };
		corner[2] = { curr.x, curr.y };
		corner[3] = { g.x, g.y };
		int maxx = corner[0].x;
		int minx = corner[0].x;
		for (int i = 0; i < 4; ++i) {
			if (maxx < corner[i].x)	maxx = corner[i].x;
			if (minx > corner[i].x)	minx = corner[i].x;
		}
		int maxy = corner[0].y;
		int miny = corner[0].y;
		for (int i = 0; i < 4; ++i) {
			if (maxy < corner[i].y)	maxx = corner[i].y;
			if (miny > corner[i].y)	minx = corner[i].y;
		}
		
		int xdist = maxx - minx;
		int ydist = maxy - miny;
		return is_blockage( 0, corner );
	}
	return false;
}

void sort(Location possible[], float d[]){
	int i = 0;
	int j = 0;
	//To sort the distances
	for (i = 0; i < 3; i++) {
		for (int j = i + 1; j < 3; j++) {
			if (d[i] > d[j]) {
				int temp = d[i];
				d[i] = d[j];
				d[j] = temp;

				Location temp1 = possible[i];
				possible[i] = possible[j];
				possible[j] = temp1;
			}
		}
	}
}
void Game::AI_move()
{
	Location possible[3];
	int or_no = 1;
	//snake's orientation check 1 -->
	if ((snake.seg[0].loc.x == (snake.seg[1].loc.x + 1)) ||
		(snake.nseg == 1)) {
		possible[0] = { 0, -1 };
		possible[1] = { 0, 1 };
		possible[2] = { 1, 0 };
		or_no = 1;
	}
	
	//Snake's Orientation Check 2 \/
	else if (snake.seg[0].loc.y == (snake.seg[1].loc.y + 1))
	{
		possible[0] = { 0,1 };
		possible[1] = { 1,0 };
		possible[2] = { -1,0 };
		or_no = 2;
	}

	//Snake's Orientation Check 3 <--
	else if (snake.seg[0].loc.x == (snake.seg[1].loc.x - 1))
	{
		possible[0] = {-1, 0};
		possible[1] = { 0,1 };
		possible[2] = { 0, -1 };
		or_no = 3;
	}

	//Snake's Orientation Check 4 /\			//
	else if (snake.seg[0].loc.y == (snake.seg[1].loc.y - 1))
	{
		possible[0] = {-1,0};
		possible[1] = {1,0};
		possible[2] = { 0,-1 };
		or_no = 4;
	}
	
	const Location next[] = { snake.getnextloc(possible[0]),
							  snake.getnextloc(possible[1]),
							  snake.getnextloc(possible[2])
							};

	float d[3] = { distance(goal.getloc(), next[0]),
		distance(goal.getloc(), next[1]),
		distance(goal.getloc(), next[2])
	};

	//wall checks
	if (!brd.IsInsideBoard(next[0])) {
		possible[0] = { 0,0 };
	}
	if (!brd.IsInsideBoard(next[1])) {
		possible[1] = { 0, 0 };
	}
	if (!brd.IsInsideBoard(next[2])) {
		possible[2] = { 0,0 };
	}

	//Self collision test
	if (snake.IsInTileExceptend(next[0])) {
		possible[0] = { 0,0 };
	}
	if (snake.IsInTileExceptend(next[1])) {
		possible[1] = { 0,0 };
	}
	if (snake.IsInTileExceptend(next[2])) {
		possible[2] = { 0,0 };
	}

	Location no_loc = { 0,0 };
	if ((possible[0] == no_loc) && (possible[1] == no_loc)
		&& (possible[2] == no_loc))
	{
		//making a random move because no matter what it chooses, he will die;
		if(or_no == 1)		del_loc = { 1,0 };
		if(or_no == 2)		del_loc = {0,1};
		if(or_no == 3)		del_loc = { -1,0 };
		if(or_no == 4)		del_loc = {0,-1};
	}
	else {
		sort(possible, d);
		Location no_loc = { 0,0 };
		bool a = possible[0] != no_loc;
		bool b = possible[1] != no_loc;
		bool c = possible[2] != no_loc;
		if ((!a && (!b || !c)) || (!b && !c)) {
			//if there is only one possible location
			if (possible[0] != no_loc) {
				del_loc = possible[0];
			}
			else if (possible[1] != no_loc) {
				del_loc = possible[1];
			}
			else {
				del_loc = possible[2];
			}
		}
		else {
			//if there are more
			bool a, b, c;
			a = b = c = true;
			if (possible[0] != no_loc) {
				a = is_future_colliding( possible[0],goal );
			}
			if (possible[1] != no_loc) {
				b = is_future_colliding( possible[1],goal );
			}
			if (possible[2] != no_loc) {
				c = is_future_colliding( possible[2],goal );
			}

			if (!a) { del_loc = possible[0]; }
			else if (!b) { del_loc = possible[1];  }
			else if(!c){ del_loc = possible[2];  }
			else {
				if (possible[2] != no_loc) {
					del_loc = possible[2];
				}
				else if (possible[1] != no_loc) {
					del_loc = possible[1];
				}
				else{
					del_loc = possible[0];
				}
			}
		}
	}

}
void Game::AI_move2() {
	Location possible[3];
	int or_no = 1;
	//snake's orientation check 1 -->
	if ((snake.seg[0].loc.x == (snake.seg[1].loc.x + 1)) ||
		(snake.nseg == 1)) {
		possible[0] = { 0, -1 };
		possible[1] = { 0, 1 };
		possible[2] = { 1, 0 };
		or_no = 1;
	}
	
	//Snake's Orientation Check 2 \/
	else if (snake.seg[0].loc.y == (snake.seg[1].loc.y + 1))
	{
		possible[0] = { 0,1 };
		possible[1] = { 1,0 };
		possible[2] = { -1,0 };
		or_no = 2;
	}

	//Snake's Orientation Check 3 <--
	else if (snake.seg[0].loc.x == (snake.seg[1].loc.x - 1))
	{
		possible[0] = {-1, 0};
		possible[1] = { 0,1 };
		possible[2] = { 0, -1 };
		or_no = 3;
	}

	//Snake's Orientation Check 4 /\			//
	else if (snake.seg[0].loc.y == (snake.seg[1].loc.y - 1))
	{
		possible[0] = {-1,0};
		possible[1] = {1,0};
		possible[2] = { 0,-1 };
		or_no = 4;
	}
	
	const Location next[] = { snake.getnextloc(possible[0]),
							  snake.getnextloc(possible[1]),
							  snake.getnextloc(possible[2])
							};

	Location g = { brd.width - goal.getloc().x, brd.height - goal.getloc().y };
	Goal fake(goal);
	fake.loc.x = g.x;
	fake.loc.y = g.y;
	float d[3] = { distance( g, next[0]),
		distance(g, next[1]),
		distance(g, next[2])
	};

	//wall checks
	if (!brd.IsInsideBoard(next[0])) {
		possible[0] = { 0,0 };
	}
	if (!brd.IsInsideBoard(next[1])) {
		possible[1] = { 0, 0 };
	}
	if (!brd.IsInsideBoard(next[2])) {
		possible[2] = { 0,0 };
	}

	//Self collision test
	if (snake.IsInTileExceptend(next[0])) {
		possible[0] = { 0,0 };
	}
	if (snake.IsInTileExceptend(next[1])) {
		possible[1] = { 0,0 };
	}
	if (snake.IsInTileExceptend(next[2])) {
		possible[2] = { 0,0 };
	}

	Location no_loc = { 0,0 };
	if ((possible[0] == no_loc) && (possible[1] == no_loc)
		&& (possible[2] == no_loc))
	{
		//making a random move because no matter what it chooses, he will die;
		if(or_no == 1)		del_loc = { 1,0 };
		if(or_no == 2)		del_loc = {0,1};
		if(or_no == 3)		del_loc = { -1,0 };
		if(or_no == 4)		del_loc = {0,-1};
	}
	else {
		sort(possible, d);
		Location no_loc = { 0,0 };
		bool a = possible[0] != no_loc;
		bool b = possible[1] != no_loc;
		bool c = possible[2] != no_loc;
		if ((!a && (!b || !c)) || (!b && !c)) {
			//if there is only one possible location
			if (possible[0] != no_loc) {
				del_loc = possible[0];
			}
			else if (possible[1] != no_loc) {
				del_loc = possible[1];
			}
			else {
				del_loc = possible[2];

			}
		}
		else {
			//if there are more
			bool a, b, c;
			a = b = c = true;
			if (possible[0] != no_loc) {
				a = is_future_colliding( possible[0],fake);
			}
			if (possible[1] != no_loc) {
				b = is_future_colliding( possible[1],fake );
			}
			if (possible[2] != no_loc) {
				c = is_future_colliding( possible[2],fake );
			}

			if (!a) { del_loc = possible[0]; }
			else if (!b) { del_loc = possible[1];  }
			else if(!c){ del_loc = possible[2];  }
			else {
				if (possible[2] != no_loc) {
					del_loc = possible[2];
				}
				else if (possible[1] != no_loc) {
					del_loc = possible[1];
				}
				else{
					del_loc = possible[0];
				}
			}
		}
	}

}
void Game::AI_move1() {
	Location possible[3];
	int or_no = 1;
	//snake's orientation check 1 -->
	if ((snake.seg[0].loc.x == (snake.seg[1].loc.x + 1)) ||
		(snake.nseg == 1)) {
		possible[0] = { 0, -1 };
		possible[1] = { 0, 1 };
		possible[2] = { 1, 0 };
		or_no = 1;
	}

	//Snake's Orientation Check 2 \/
	else if (snake.seg[0].loc.y == (snake.seg[1].loc.y + 1))
	{
		possible[0] = { 0,1 };
		possible[1] = { 1,0 };
		possible[2] = { -1,0 };
		or_no = 2;
	}

	//Snake's Orientation Check 3 <--
	else if (snake.seg[0].loc.x == (snake.seg[1].loc.x - 1))
	{
		possible[0] = { -1, 0 };
		possible[1] = { 0,1 };
		possible[2] = { 0, -1 };
		or_no = 3;
	}

	//Snake's Orientation Check 4 /\			//
	else if (snake.seg[0].loc.y == (snake.seg[1].loc.y - 1))
	{
		possible[0] = { -1,0 };
		possible[1] = { 1,0 };
		possible[2] = { 0,-1 };
		or_no = 4;
	}

	const Location next[] = { snake.getnextloc(possible[0]),
		snake.getnextloc(possible[1]),
		snake.getnextloc(possible[2])
	};
	float d[3] = { distance(goal.getloc(), next[0]),
	distance(goal.getloc(), next[1]),
	distance(goal.getloc(), next[2])
	};

	//wall checks
	if (!brd.IsInsideBoard(next[0])) {
		possible[0] = { 0,0 };
	}
	if (!brd.IsInsideBoard(next[1])) {
		possible[1] = { 0, 0 };
	}
	if (!brd.IsInsideBoard(next[2])) {
		possible[2] = { 0,0 };
	}

	//Self collision test
	if (snake.IsInTileExceptend(next[0])) {
		possible[0] = { 0,0 };
	}
	if (snake.IsInTileExceptend(next[1])) {
		possible[1] = { 0,0 };
	}
	if (snake.IsInTileExceptend(next[2])) {
		possible[2] = { 0,0 };
	}

	Location no_loc = { 0,0 };

	sort(possible,d);
	if ((possible[0] == no_loc) && (possible[1] == no_loc)
		&& (possible[2] == no_loc))
	{
		//making a random move because no matter what it chooses, he will die;
		if (or_no == 1)		del_loc = { 1,0 };
		if (or_no == 2)		del_loc = { 0,1 };
		if (or_no == 3)		del_loc = { -1,0 };
		if (or_no == 4)		del_loc = { 0,-1 };
	}
	else {
		int diff_x = (goal.getloc().x - snake.seg[0].loc.x);
		int diff_y = (goal.getloc().y - snake.seg[0].loc.y);
		int fail_y = 0;
		int fail_x = 0;
		if (diff_y != 0) {
			if ((possible[0].y == 0) && (possible[1].y == 0) && (possible[2].y == 0)) {
				fail_y = 1;
			}
			else {
				int comp = 1;
				if (diff_y > 0) {
					comp = 1;
				}
				else
				{
					comp = -1;
				}
				int i;
				
				for (i = 0; i < 3; i++)
				{
					if ((possible[i].x == 0) && (possible[i].y == comp)) {
						bool a = is_future_colliding(possible[i],goal);
						if (!a) {
							del_loc = possible[i];
							break;
						}
						else {
							fail_y = 1;
						}
					}
				}
				if (i == 3) {
					fail_y = 1;
				}
				
			}
			
		}
		else if (diff_x != 0 || fail_y == 1) {
			if (diff_x != 0) {
				int i;
				int comp = 1;
				if (diff_x > 0) {
					comp = 1;
				}
				else	comp = -1;
				
				for (i = 0; i < 3; i++)
				{
					if ((possible[i].y == 0) && (possible[i].x == comp)) {
						bool a = is_future_colliding(possible[i],goal);
						if (!a) {
							del_loc = possible[i];
							break;
						}
						else {
							fail_x = 1;
						}
					}
				}
				
			}
			if (fail_y && fail_x) {
				bool a, b, c;
				a = b = c = true;
				if (possible[0] != no_loc) {
					a = is_future_colliding(possible[0],goal);
				}
				if (possible[1] != no_loc) {
					b = is_future_colliding(possible[1],goal);
				}
				if (possible[2] != no_loc) {
					c = is_future_colliding(possible[2],goal);
				}

				if (!a) { del_loc = possible[0]; }
				else if (!b) { del_loc = possible[1]; }
				else if (!c) { del_loc = possible[2]; }
				else {
					if (possible[2] != no_loc) {
						del_loc = possible[2];
					}
					else if (possible[1] != no_loc) {
						del_loc = possible[1];
					}
					else {
						del_loc = possible[0];
					}
				}
			}

			
		}
	}
}
/*
{
	Location possible[3];
	int or_no = 1;
	//snake's orientation check 1 -->
	if ((snake.seg[0].loc.x == (snake.seg[1].loc.x + 1)) ||
		(snake.nseg == 1)) {
		possible[0] = { 0, -1 };
		possible[1] = { 0, 1 };
		possible[2] = { 1, 0 };
		or_no = 1;
	}

	//Snake's Orientation Check 2 \/
	else if (snake.seg[0].loc.y == (snake.seg[1].loc.y + 1))
	{
		possible[0] = { 0,1 };
		possible[1] = { 1,0 };
		possible[2] = { -1,0 };
		or_no = 2;
	}

	//Snake's Orientation Check 3 <--
	else if (snake.seg[0].loc.x == (snake.seg[1].loc.x - 1))
	{
		possible[0] = { -1, 0 };
		possible[1] = { 0,1 };
		possible[2] = { 0, -1 };
		or_no = 3;
	}

	//Snake's Orientation Check 4 /\			//
	else if (snake.seg[0].loc.y == (snake.seg[1].loc.y - 1))
	{
		possible[0] = { -1,0 };
		possible[1] = { 1,0 };
		possible[2] = { 0,-1 };
		or_no = 4;
	}

	const Location next[] = { snake.getnextloc(possible[0]),
		snake.getnextloc(possible[1]),
		snake.getnextloc(possible[2])
	};

	//wall checks
	if (!brd.IsInsideBoard(next[0])) {
		possible[0] = { 0,0 };
	}
	if (!brd.IsInsideBoard(next[1])) {
		possible[1] = { 0, 0 };
	}
	if (!brd.IsInsideBoard(next[2])) {
		possible[2] = { 0,0 };
	}

	//Self collision test
	if (snake.IsInTileExceptend(next[0])) {
		possible[0] = { 0,0 };
	}
	if (snake.IsInTileExceptend(next[1])) {
		possible[1] = { 0,0 };
	}
	if (snake.IsInTileExceptend(next[2])) {
		possible[2] = { 0,0 };
	}

	Location no_loc = { 0,0 };
	if ((possible[0] == no_loc) && (possible[1] == no_loc)
		&& (possible[2] == no_loc))
	{
		//making a random move because no matter what it chooses, he will die;
		if (or_no == 1)		del_loc = { 1,0 };
		if (or_no == 2)		del_loc = { 0,1 };
		if (or_no == 3)		del_loc = { -1,0 };
		if (or_no == 4)		del_loc = { 0,-1 };
	}
	else {
		int diff_x = (goal.getloc().x - snake.seg[0].loc.x);
		int diff_y = (goal.getloc().y - snake.seg[0].loc.y);
		int fail_y = 0;
		
		if (diff_y != 0) {
			if ((possible[0].y == 0) && (possible[1].y == 0) && (possible[2].y == 0)) {
				fail_y = 1;
			}
			else if (diff_y > 0) {
				int i;
				for (i = 0; i < 3; i++) 
				{
						if ((possible[i].x == 0) && (possible[i].y == 1)) {
							int d = abs(diff_y);
							for (int k = snake.seg[0].loc.y; d != 0; k++) {
								if (snake.IsInTileExceptend( { snake.seg[0].loc.x, k } )) {
									fail_y = 1;
									break;
								}
								d--;
							}
							if (d == 0) {
								del_loc = possible[i];
								break;
							}
						}
				}
				if (i == 3) {
					fail_y = 1;
				}
			}
			else {
				int i;
				for (i = 0; i < 3; i++)
				{
					if ((possible[i].x == 0) && (possible[i].y == -1)) {
						int d = abs(diff_y);
						for (int k = snake.seg[0].loc.y; d != 0; k--) {
							if (snake.IsInTileExceptend({ snake.seg[0].loc.x, k })) {
								fail_y = 1;
								break;
							}
							d--;
						}
						if (d == 0) {
							del_loc = possible[i];
							break;
						}
					}
				}
				if (i == 3) {
					fail_y = 1;
				}
			}
		}
		else if (diff_x != 0 || fail_y == 1) {
			if (diff_x > 0) {
				int i;
				for (i = 0; i < 3; i++)
				{
					if ((possible[i].y == 0) && (possible[i].x == 1)) {
						del_loc = possible[i];
						break;
					}
				}
			}
			else {
				int i;
				for (i = 0; i < 3; i++)
				{
					if ((possible[i].y == 0) && (possible[i].x == -1)) {
						del_loc = possible[i];
						break;
					}
				}
			}
		}
	}
}*/
float Game::distance(Location l1, Location l2)
{
	float X = (l2.x - l1.x)*(l2.x - l1.x);
	float Y = (l2.y - l1.y)*(l2.y - l1.y);
	float Z = X + Y;
	float d = sqrt( Z );
	return d;
}



void Game::ComposeFrame()
{
	brd.DrawBorder();
	snake.Draw(brd);
	goal.Draw(brd);
	
	if (gameisover) {
		SpriteCodex::DrawGameOver(400, 300, gfx);
		if(wnd.kbd.KeyIsPressed(VK_SPACE)) {
			gameisover = false;
			reset();
		}
	}

}
