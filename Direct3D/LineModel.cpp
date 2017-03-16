#include "IncludePCH.h"
#include "LineModel.h"

LineModel::LineModel()
{
}

LineModel::~LineModel()
{
}

LineModel::LineModel(std::vector<Geometry::ColorVertex> verts)
{
	std::vector<DWORD> inds;
	for (size_t i = 0; i < verts.size(); i++)
	{
		inds.push_back(i);
	};
	m_mesh = std::make_unique<MeshFactory::Mesh<Geometry::ColorVertex>>(verts, inds);

}

LineModel::LineModel(std::vector<Geometry::ColorVertex> verts, DirectX::XMFLOAT4 color)
{
	std::vector<DWORD> inds;
	for (size_t i = 0; i < verts.size(); i++)
	{
		inds.push_back(i);
		verts[i].color = color;
	};
	m_mesh = std::make_unique<MeshFactory::Mesh<Geometry::ColorVertex>>(verts, inds);

	
}

MeshFactory::Mesh<Geometry::ColorVertex>* LineModel::GetMesh()
{
	return m_mesh.get();
}
