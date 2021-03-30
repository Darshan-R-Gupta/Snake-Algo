#pragma once
#include "Board.h"

class Snake {
private:
	class Segment {
	public:
		void InitHead(const Location &in_loc, Color col);
		void InitBody(Color col);
		void Move(const Location &dloc);
		void Follow(const Segment & next);
		void Draw(Board & brd) const;
		const Location& getloc() const;
		const int thick = 6;
		Location loc;
		Color c;
	};

public:
	Snake(const Location &loc);
	void Move(const Location & dloc);
	void Grow();
	void resetsnake(Board &brd);
	void Draw(Board & brd) const;
	Location getnextloc(const Location &dloc) const;
	int IsInTileExceptend(const Location &target, int = -1) const;
	int IsInTile(const Location &target, int = -1) const;
	
public:
	static constexpr int maxseg = 200;
	Segment seg[maxseg];
	int nseg = 2;
};