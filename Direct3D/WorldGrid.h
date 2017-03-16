#pragma once
#include "Table.h"


struct alignas(16) WorldGridCell
{
	std::vector<class Object3D*> occupants;
	bool passable = true;
	DirectX::XMFLOAT3 center;
	static float width;
	static float height;
};
class WorldGrid
{
	friend class MiniMap;
	float m_width;
	float m_height;
	float m_cellHeight;
	float m_cellWidth;
	int m_rows;
	int m_columns;
	

	DirectX::XMINT2 getRowColumn(DirectX::XMFLOAT3 position);
public:
	std::array<int, 3> m_cellArray3 = { -1,0,1 };
	std::array<int, 5> m_cellArray5 = { -2,-1,0,1,2 };
	std::array<int, 7> m_cellArray7 = { -3,-2,-1,0,1,2,3 };
	std::array<int, 9> m_cellArray9 = { -4,-3,-2,-1,0,1,2,3,4 };
	
	const int max_array_size = 9;
public:
	WorldGrid() {}
	WorldGrid(const float width, const float height, const float cellWidth, const float cellHeight);
	WorldGridCell* GetCell(DirectX::XMFLOAT3 position);
	bool RemoveFromCell(class Object3D* obj);
	bool AddObject(class Object3D* obj);
	bool AddObject(class Object3D* obj, WorldGridCell* cell);
	bool UpdateCell(class Object3D* obj);
private:
	Table<WorldGridCell> m_cells;
};


