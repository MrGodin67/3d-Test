#pragma once


#include "Camera3D_2.h"
#include "Utilities.h"
class FoliageClass
{
	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
		VertexType() {}
		VertexType(DirectX::XMFLOAT3 p, DirectX::XMFLOAT2 t)
			:position(p), texture(t)
		{}
	};

	struct FoliageType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 color;
	};

	struct InstanceType
	{
		DirectX::XMMATRIX matrix;
		DirectX::XMFLOAT3 color;
	};
	ID3D11Buffer* m_vertexBuffer = nullptr;
	ID3D11Buffer* m_InstanceBuffer = nullptr;
	ID3D11ShaderResourceView* m_texture = nullptr;
	UINT m_numInstances;
	UINT m_VisibleCount;
	std::vector<InstanceType> m_Instances;
	std::vector<FoliageType> m_data;
	Utilities::BoundingBox m_aabb;
public:
	FoliageClass(const std::string texFilename,std::vector<DirectX::XMFLOAT3>& positions);
	~FoliageClass();
	ID3D11Buffer* GetVertexBuffer();
	ID3D11Buffer* GetInstanceBuffer();
	ID3D11ShaderResourceView* GetTexture();
	UINT GetVisibleCount();
	void CheckInView( CFirstPersonCamera& cam);
	UINT InstanceStructSize(){ return sizeof(InstanceType); }
	UINT VertexStructSize() { return sizeof(VertexType); }
};