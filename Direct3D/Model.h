#pragma once

#include "MeshFactory.h"
#include "tiny_obj_loader2.h"
#include "Locator.h"
#include "Utilities.h"
class WavefrontModel
{
private:
	DirectX::XMFLOAT3 m_WHD;
	std::vector<MeshFactory::Mesh<Geometry::LOD_VertexMin>> m_meshes;
	std::vector<tinyobj::material_t> m_materials;
	Utilities::BoundingBox aabb;
	std::vector<ID3D11ShaderResourceView*> m_textures;
public:
	WavefrontModel();
	virtual ~WavefrontModel() 
	{
		for (auto& it : m_textures)
			SafeRelease(it);
	}
	HRESULT LoadFromFile(std::string filename);
	size_t MeshCount();
	size_t MaterialCount();
	std::string GetMaterialDiffuseName(const size_t index);
	MeshFactory::Mesh<Geometry::LOD_VertexMin>* GetMesh(const size_t index);
	
	void Release();
	float Width();
	float Height();
	float Depth();
	Utilities::BoundingBox BoundingBox();
	/*///<summary>
	/// Model Space
	///</summary>
	
	Tools3D::AABB_3D BoundingBox();
	PlaneIntersect  RayIntersectMeshMouse( XMFLOAT2 mouse, D3D11_VIEWPORT& vp, XMMATRIX world,XMMATRIX view, XMMATRIX projection);*/
};