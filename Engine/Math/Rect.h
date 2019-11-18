#pragma once
#include "MathPCH.h"

class MyRect
{
public:
	int x;
	int y;
	int w;
	int h;

public:
	MyRect() {}
	MyRect(int nx, int ny, int nw, int nh) { x = nx; y = ny; w = nw; h = nh; }

	void Set(int nx, int ny, int nw, int nh) { x = nx; y = ny; w = nw; h = nh; }
};