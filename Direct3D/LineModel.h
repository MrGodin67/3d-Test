#pragma once

#include "Geometry.h"
#include "MeshFactory.h"

class LineModel
{
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	std::unique_ptr<MeshFactory::Mesh<Geometry::ColorVertex>> m_mesh;

public:
	LineModel();
	~LineModel();
	LineModel(std::vector<Geometry::ColorVertex> verts);
	LineModel(std::vector<Geometry::ColorVertex> verts,DirectX::XMFLOAT4 color);
	MeshFactory::Mesh<Geometry::ColorVertex>* GetMesh();
	

};
