#pragma once
#include<vector>
using namespace std;

class CGameObject;

class CGridObjects
{
private:
	const int GRID_CELL_WIDTH = 64;
	const int GRID_CELL_HEIGHT = 64;

	int rowsCount, colsCount;

	/// <summary>
	/// Large objects are the ones that cannot fit into any cells
	/// </summary>
	vector<CGameObject*> largeObjs;

	/// [row][col][obj]
	vector<vector<vector<CGameObject*>>> cells;

	void CalcGridRange(int areaX, int areaY, int areaWidth, int areaHeight, int& leftColumn, int& topRow, int& rightColumn, int& bottomRow);

	bool IsLargeObject(CGameObject* obj);

	/// <summary>
	/// CuteTN Note:
	/// remove game object from a specific cell node. This is also used to remove a game object from its old cell after updating to a new cell
	/// </summary>
	/// <param name="obj"></param>
	/// <param name="row"></param>
	/// <param name="col"></param>
	/// <param name="deleteAfterRemoving"></param>
	void RemoveFromCell(CGameObject* obj, int row, int col, bool deleteAfterRemoving);

	void RemoveFromLargeObjects(CGameObject* obj, bool deleteAfterRemoving);


public:
	CGridObjects() {}
	CGridObjects(int sectionWidth, int sectionHeight);

	void CalcCell(int& row, int& col, int x, int y);
	void CalcCell(int& row, int& col, CGameObject* obj);

	void Add(CGameObject* obj);
	void Remove(CGameObject* obj, bool deleteAfterRemoving);

	/// <summary>
	/// this function tries to seek the object any where on the grid. It cannot hide :^)
	/// </summary>
	/// <param name="obj"></param>
	void SeekAndRemove(CGameObject* obj, bool deleteAfterRemoving);
	void Clear();

	vector<CGameObject*> GetObjectsInArea(int areaX, int areaY, int areaWidth, int areaHeight);
	vector<CGameObject*> GetAllObjects();
	void ClearObjectsInArea(int dt, int areaX, int areaY, int areaWidth, int areaHeight);
};

typedef CGridObjects* LPGRIDOBJECTS;
