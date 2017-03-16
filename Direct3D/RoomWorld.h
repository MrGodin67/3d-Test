#pragma once

#include "Utilities.h"
#include <vector>
#include <DirectXMath.h>
#include <algorithm>   
struct RoomCell
{
	std::vector<Utilities::Plane> m_planes;
	DirectX::XMFLOAT2 pos;
};
class RoomWorld
{
	float m_width, m_height, m_cellSize;
	std::vector<RoomCell> m_cells;
	std::vector<std::unique_ptr<Utilities::DebugTri>> m_dbTri;
	size_t m_columns;
	size_t m_rows;
public:
	RoomWorld() = default;
	~RoomWorld() {}
	RoomWorld(Utilities::BoundingBox& box, float cellSize)
	{
		m_width = (box.maxPt.x - box.minPt.x) + 0.5f;
		m_height = (box.maxPt.z - box.minPt.z) + 0.5f;
		m_cellSize = cellSize;
		float startColumn = 0.0f;// box.minPt.x;
		float startRow = 0.0f;// box.minPt.z;
		m_rows = (int)(m_height / m_cellSize)+1;
		m_columns = (int)(m_width / m_cellSize)+1;
		m_cells.resize(m_rows*m_columns);
		for (size_t r = 0; r < m_rows; r++)
		{
			startColumn = box.minPt.x;
			for (size_t c = 0; c < m_columns; c++)
			{
				const size_t index = r * m_columns + c;
				m_cells[index].pos = { startColumn,startRow };
				startColumn += m_cellSize;
			}
			startRow += m_cellSize;
		}
	}
	
	void AddPlane(Utilities::Plane* plane)
	{
		
		// ?? hmm.. adding one plane to possible 3 cells, i think
		// i have to do this in order to make sure
		// we can intersect that plane, no matter how 
		// little of it is in the cell

		// get indices of plane points
		

		m_dbTri.push_back(std::make_unique<Utilities::DebugTri>(*plane, plane->box));

		size_t index[3];
		size_t cSize = (size_t)m_cellSize;
		size_t row1 =  (size_t)plane->pt0.z / cSize;
		size_t col1 = (size_t)plane->pt0.x / cSize;

		size_t row2 = (size_t)plane->pt1.z / cSize;
		size_t col2 = (size_t)plane->pt1.x / cSize;

		size_t row3 = (size_t)plane->pt2.z / cSize;
		size_t col3 = (size_t)plane->pt2.x / cSize;

		index[0] = row1 * m_columns + col1;
		index[1] = row2 * m_columns + col2;
		index[2] = row3 * m_columns + col3;
		Utilities::Plane a;
		Utilities::Plane b;
		for (size_t i = 0; i < 3; i++)
		{
			
				// not in cell's plane list so add it
				
				auto& it = std::find_if(m_cells[index[i]].m_planes.begin(),
					m_cells[index[i]].m_planes.end(), [plane](Utilities::Plane& p) 
				{
					return plane->index == p.index;

				});
				if(it == m_cells[index[i]].m_planes.end())
				   m_cells[index[i]].m_planes.push_back(*plane);
			}
			
		
	}
	RoomCell* getCell(DirectX::XMFLOAT3 pos)
	{
		int index = (size_t)((size_t)pos.z / (size_t)m_cellSize) * m_columns + (size_t)((size_t)pos.x / (size_t)m_cellSize);
		if (index < 0 || (size_t)index >= m_cells.size())
			return nullptr;
		return &m_cells[index];
	}
	void Render(ShaderFactory::Shader* shader)
	{
		for (auto& it : m_dbTri)
		{
			it->Rasterize(shader, nullptr);
		}
	}
};