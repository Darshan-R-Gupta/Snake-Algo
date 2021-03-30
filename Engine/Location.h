#pragma once
class Location {
public:
	Location() {
		x = 0;
		y = 0;
	}
	Location(int x1, int y1) {
		x = x1;
		y = y1;
	}
	void Add(const Location& val) {
		x += val.x;
		y += val.y;
	}
	void change(const Location& val) {
		x = val.x;
		y = val.y;
	}
	bool operator ==(const Location& rhs) const {
		return x == rhs.x && y == rhs.y;
	}
	bool operator !=(const Location& rhs) const {
		return !(x == rhs.x && y == rhs.y);
	}
	int x, y;
};