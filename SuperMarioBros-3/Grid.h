#pragma once
#include "GameObject.h"
#include "Game.h"

#define CELL_WIDTH	150
#define CELL_HEIGHT	180

#define MAP_WIDTH	3328
#define MAP_HEIGHT	1000

class CGrid
{
	vector<vector<vector<LPGAMEOBJECT>>> cells;
public:
	CGrid();
	~CGrid();
	void Resize();
	void Clear(int numRow, int numCol);
	void Push(LPGAMEOBJECT obj, int row, int col);
	void Update(vector<LPGAMEOBJECT> listObj);
	void Get(vector<LPGAMEOBJECT>& listObj);
	void MakeObjectsOutCam(vector<LPGAMEOBJECT>& listObj);
};

