#include "IncludePCH.h"
#include "WorldGrid.h"
#include "Object3D.h"

using namespace DirectX;
float WorldGridCell::height = 10.0f;
float WorldGridCell::width = 10.0f;
DirectX::XMINT2 WorldGrid::getRowColumn(DirectX::XMFLOAT3 position)
{
	int row = (int)((256.0f - position.z) / m_cellHeight);
	int col = (int)(position.x / m_cellWidth);
	return DirectX::XMINT2(col, row);
}

WorldGrid::WorldGrid(const float width, const float height, const float cellWidth, const float cellHeight)
	:m_width(width), m_height(height), m_cellWidth(cellWidth), m_cellHeight(cellHeight)
{
	m_columns = (int)(m_width / m_cellWidth);
	m_rows = (int)(m_height / m_cellHeight);
	m_cells.resize(m_rows, m_columns);
	for (int r = 0; r < m_rows; r++)
	{
		for (int c = 0; c < m_columns; c++)
		{
			float x = (float)((c * m_cellWidth) + (m_cellWidth / 2));
			float z = m_height - (float)((r * m_cellHeight) + (m_cellHeight / 2));
			m_cells(r, c).center = XMFLOAT3(x, 0.0f, z);
		}
	}
	
}

WorldGridCell * WorldGrid::GetCell(DirectX::XMFLOAT3 position)
{
	DirectX::XMINT2 data = getRowColumn(position);
	const int index = data.y * m_columns + data.x;
	if (index < 0 || index >= (m_rows * m_columns))
	{
		return nullptr;
	}
	return &m_cells(data.y, data.x);
}

bool WorldGrid::RemoveFromCell(Object3D* obj)
{
	if (obj->pCurrentCell)
	{
		auto& it = std::find(obj->pCurrentCell->occupants.begin(),
			obj->pCurrentCell->occupants.end(), obj);

		if (it != obj->pCurrentCell->occupants.end())
		{
			obj->pCurrentCell->occupants.erase(it);
			obj->pCurrentCell = nullptr;
			return true;
		}
		else
		{
			return false;
		}
	}
	return true;
}

bool WorldGrid::AddObject(Object3D * obj)
{
	if (obj->pCurrentCell)
	{
		if (!RemoveFromCell(obj))
		{
			return false;
		};
	}
	DirectX::XMFLOAT3 pos;
	XMStoreFloat3(&pos, obj->GetPosition());
	obj->pCurrentCell = GetCell(pos);
	obj->pCurrentCell->occupants.push_back(obj);

	return true;
}

bool WorldGrid::AddObject(Object3D * obj, WorldGridCell * cell)
{
	if (obj->pCurrentCell)
	{
		if (!RemoveFromCell(obj))
		{
			return false;
		};
	}
	obj->pCurrentCell = cell;
	obj->pCurrentCell->occupants.push_back(obj);

	return false;
}

bool WorldGrid::UpdateCell(Object3D * obj)
{
	DirectX::XMFLOAT3 pos;
	XMStoreFloat3(&pos, obj->GetPosition());
	WorldGridCell* cell = GetCell(pos);
	if (cell == obj->pCurrentCell)
		return true;
	bool result;
	result = RemoveFromCell(obj);
	if (result)
		result = AddObject(obj, cell);

	assert(result == true);
	return result;
}