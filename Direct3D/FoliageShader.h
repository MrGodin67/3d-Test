#pragma once
#include "ShaderFactory.h"
class FoliageShader :
	public ShaderFactory::Shader
{
	int m_VisibleCount = 0;
	const UINT numElements = sizeof(ShaderFactory::InputLayoutDesc::InstancedWorld32) /
		sizeof(ShaderFactory::InputLayoutDesc::InstancedWorld32[0]);
	
public:
	FoliageShader(ShaderFactory::InitShaderDesc desc);
	~FoliageShader();
	void SetVisibleCount(const UINT count)override;
	// Inherited via Shader
	virtual HRESULT Render(ID3D11DeviceContext * context, ID3D11Buffer * VBuffer, ID3D11Buffer * IBuffer, const UINT vertStructInBytes, const int indexCount, const std::vector<ID3D11ShaderResourceView*> textures) override;
};

