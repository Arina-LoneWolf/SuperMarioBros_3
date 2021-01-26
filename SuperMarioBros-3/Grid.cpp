#include "Grid.h"
#include "Koopa.h"

CGrid::CGrid()
{
}

CGrid::~CGrid()
{
}

void CGrid::Resize()
{
	int totalRow = MAP_HEIGHT / CELL_HEIGHT;
	int totalCol = MAP_WIDTH / CELL_WIDTH;

	cells.resize(totalRow + 1);
	for (int i = 0; i < totalRow + 1; i++)
		cells[i].resize(totalCol + 1);

	Clear(totalRow, totalCol);
}

void CGrid::Clear(int numRow, int numCol)
{
	for (int i = 0; i < numRow; i++)
		for (int j = 0; j < numCol; j++)
			cells[i][j].clear();
}

void CGrid::Push(LPGAMEOBJECT obj, int row, int col)
{
	cells[row][col].push_back(obj);
}

void CGrid::Update(vector<LPGAMEOBJECT> listObj)
{
	Clear(MAP_HEIGHT / CELL_HEIGHT, MAP_WIDTH / CELL_WIDTH);

	for (int i = 0; i < listObj.size(); i++)
	{
		float l, t, r, b;
		listObj[i]->GetBoundingBox(l, t, r, b);

		int cellTop = int(t / CELL_HEIGHT);
		int cellBottom = ceil(b / CELL_HEIGHT);
		int cellLeft = int(l / CELL_WIDTH);
		int cellRight = ceil(r / CELL_WIDTH);

		if (!listObj[i]->isFinishedUsing)
		{
			for (int row = cellTop; row < cellBottom; row++)
				for (int col = cellLeft; col < cellRight; col++)
					cells[row][col].push_back(listObj[i]);
		}
	}
}

void CGrid::Get(vector<LPGAMEOBJECT>& listObj)
{
	int firstCol = (int)(CGame::GetInstance()->GetCamPosX() / CELL_WIDTH);
	int lastCol = (int)((CGame::GetInstance()->GetCamPosX() + SCREEN_WIDTH) / CELL_WIDTH);
	int totalRow = MAP_HEIGHT / CELL_HEIGHT;

	for (int i = 0; i < totalRow; i++)
	{
		for (int j = firstCol; j < lastCol; j++)
		{
			for (int k = 0; k < cells[i][j].size(); k++)
			{
				if (cells[i][j][k]!=NULL)
				if (!cells[i][j][k]->onCam && !cells[i][j][k]->isFinishedUsing)
				{
					cells[i][j][k]->onCam = true;
					listObj.push_back(cells[i][j][k]);
				}
			}
		}
	}
}

void CGrid::MakeObjectsOutCam(vector<LPGAMEOBJECT>& listObj)
{
	for (LPGAMEOBJECT obj : listObj)
		obj->onCam = false;
}
