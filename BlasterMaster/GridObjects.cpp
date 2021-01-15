#include "GridObjects.h"
#include "GameObjectBehaviour.h"

CGridObjects::CGridObjects(int sectionWidth, int sectionHeight)
{
	rowsCount = sectionHeight / GRID_CELL_HEIGHT;
	colsCount = sectionWidth / GRID_CELL_WIDTH;
			
	cells.resize(rowsCount);
	for (int r = 0; r < rowsCount; r++)
	{
		cells[r].resize(colsCount);

		for (int c = 0; c < colsCount; c++)
		{
			cells[r][c].clear();
		}
	}
}

void CGridObjects::CalcCell(int& row, int& col, int x, int y)
{
	col = x / GRID_CELL_WIDTH;
	row = y / GRID_CELL_HEIGHT;

	col = max(col, 0);
	row = max(row, 0);
	col = min(col, colsCount - 1);
	row = min(row, rowsCount - 1);
}

void CGridObjects::CalcCell(int& row, int& col, CGameObject* obj)
{
	if (!obj)
		row = col = -1;

	float cenX, cenY;
	CGameObjectBehaviour::CalcBoundingBoxCenter(obj, cenX, cenY);

	CalcCell(row, col, cenX, cenY);
}

void CGridObjects::Add(CGameObject* obj)
{
	if (!obj)
		return;

	if (IsLargeObject(obj))
	{
		largeObjs.push_back(obj);
		return;
	}

	int col, row;
	CalcCell(row, col, obj);

	if (col == -1 || row == -1)
		return;

	cells[row][col].push_back(obj);
}

void CGridObjects::Remove(CGameObject* obj, bool deleteAfterRemoving)
{
	if (!obj)
		return;

	if (IsLargeObject(obj))
	{
		RemoveFromLargeObjects(obj, deleteAfterRemoving);
		return;
	}

	int col, row;
	CalcCell(row, col, obj);

	if (col == -1 || row == -1)
		return;

	RemoveFromCell(obj, row, col, deleteAfterRemoving);
}

void CGridObjects::SeekAndRemove(CGameObject* obj, bool deleteAfterRemoving)
{
	if (!obj)
		return;

	if (IsLargeObject(obj))
	{
		RemoveFromLargeObjects(obj, deleteAfterRemoving);
		return;
	}

	for (int r = 0; r < rowsCount; r++)
	{
		for (int c = 0; c < colsCount; c++)
		{
			RemoveFromCell(obj, r, c, deleteAfterRemoving);
		}
	}
}

void CGridObjects::Clear()
{
	for (int r = 0; r < rowsCount; r++)
	{
		for (int c = 0; c < colsCount; c++)
		{
			cells[r][c].clear();
		}
		cells[r].clear();
	}
	cells.clear();

	largeObjs.clear();
}

vector<CGameObject*> CGridObjects::GetObjectsInArea(int dt, int areaX, int areaY, int areaWidth, int areaHeight)
{
	int leftColumn, topRow, rightColumn, bottomRow;
	CalcGridRange(areaX, areaY, areaWidth, areaHeight, leftColumn, topRow, rightColumn, bottomRow);

	// CuteTN Note:
	// Shallow copy a vector I think...
	// rep: yeah I was right
	vector<LPGAMEOBJECT> result = largeObjs;

	for (int r = topRow; r <= bottomRow; r++)
		for (int c = leftColumn; c <= rightColumn; c++)
		{
			for (auto obj : cells[r][c])
				result.push_back(obj);
		}
	
	return result;
}

void CGridObjects::ClearObjectsInArea(int dt, int areaX, int areaY, int areaWidth, int areaHeight)
{
	int leftColumn, topRow, rightColumn, bottomRow;
	CalcGridRange(areaX, areaY, areaWidth, areaHeight, leftColumn, topRow, rightColumn, bottomRow);

	for (int r = topRow; r <= bottomRow; r++)
		for (int c = leftColumn; c <= rightColumn; c++)
		{
			cells[r][c].clear();
		}

	largeObjs.clear();
}

void CGridObjects::CalcGridRange(int areaX, int areaY, int areaWidth, int areaHeight, int& leftColumn, int& topRow, int& rightColumn, int& bottomRow)
{
	CalcCell(topRow, leftColumn, areaX - GRID_CELL_WIDTH, areaY - GRID_CELL_HEIGHT);
	CalcCell(bottomRow, rightColumn, areaX + areaWidth + GRID_CELL_WIDTH, areaY + areaHeight + GRID_CELL_HEIGHT);
}

bool CGridObjects::IsLargeObject(CGameObject* obj)
{
	float l, t, r, b;
	obj->GetBoundingBox(l, t, r, b);

	float w = r - l;
	float h = b - t;

	return (w > GRID_CELL_WIDTH) || (h > GRID_CELL_HEIGHT);
}

void CGridObjects::RemoveFromCell(CGameObject* obj, int row, int col, bool deleteAfterRemoving)
{
	for (int i = 0; i < cells[row][col].size(); i++)
		if (cells[row][col][i] == obj)
		{
			if (deleteAfterRemoving)
			{
				delete cells[row][col][i];
				cells[row][col][i] = nullptr;
			}

			cells[row][col].erase(cells[row][col].begin() + i);
			break;
		}
}

void CGridObjects::RemoveFromLargeObjects(CGameObject* obj, bool deleteAfterRemoving)
{
	for (int i = 0; i < largeObjs.size(); i++)
		if (largeObjs[i] == obj)
		{
			if (deleteAfterRemoving)
			{
				delete largeObjs[i];
				largeObjs[i] = nullptr;
			}

			largeObjs.erase(largeObjs.begin() + i);
			break;
		}
}

//void CGridObjects::UpdateInterestingArea(int dt, int areaX, int areaY, int areaWidth, int areaHeight)
//{
//
//
//
//
//	for (auto obj : interestingObjs)
//	{
//		if (!obj)
//			continue;
//
//		if (dynamic_cast<LPTILE_AREA>(obj))
//			obj->Update(dt, nullptr);
//		else
//		{
//			if (obj->isUpdatedWhenOffScreen || checkObjInCamera(obj, SCREEN_EXTEND_OFFSET_DEFAULT))
//			{
//				obj->Update(dt, &interestingObjs);
//			}
//		}
//	}
//}
