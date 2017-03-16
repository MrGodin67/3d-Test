#include "IncludePCH.h"
#include "FoliageShader.h"
using namespace DirectX;

FoliageShader::FoliageShader(ShaderFactory::InitShaderDesc desc)
{
	ID3DBlob* vblob;
	ID3DBlob* pblob;
	m_vertexShader = ShaderFactory::LoadShader<ID3D11VertexShader>(&vblob, desc.first);
	assert(m_vertexShader);// check pointer
	m_pixelShader = ShaderFactory::LoadShader<ID3D11PixelShader>(&pblob, desc.second);
	assert(m_pixelShader);
	ShaderFactory::SamplerStates::CreateSamplerState(ShaderFactory::SamplerStates::BasicTexture, &m_sampler);
	assert(m_sampler);
	ShaderFactory::InputLayoutDesc::CreateLayout(ShaderFactory::InputLayoutDesc::InstancedWorld32, numElements, vblob, &m_inputLayout);
	assert(m_inputLayout);
	SafeRelease(vblob);
	SafeRelease(pblob);


}


FoliageShader::~FoliageShader()
{
}
void FoliageShader::SetVisibleCount(UINT count)
{
	m_VisibleCount = count;
}
///<summary>
/// verStructInByte = VertType : indexCount = Instance Matrix/color struct
///</summary>
HRESULT FoliageShader::Render(ID3D11DeviceContext * context, ID3D11Buffer * VBuffer, ID3D11Buffer * IBuffer, const UINT vertStructInBytes, const int indexCount, const std::vector<ID3D11ShaderResourceView*> textures)
{
	
	//context->PSSetConstantBuffers(0, 1, &ShaderFactory::ConstFogLightBuffers[ShaderFactory::CB_light]);
	UINT stride[2] = { vertStructInBytes, (UINT)indexCount };
	UINT offset[2] = { 0,0 };
	ID3D11Buffer* vbs[2] = { VBuffer, IBuffer};
	//// set buffers into pipeline
	context->IASetVertexBuffers(0, 2, vbs, stride, offset);
	//context->IASetIndexBuffer(m_model->GetMesh(c)->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
	//UINT offset = 0;
	//context->IASetVertexBuffers(0, 1, &VBuffer, &vertStructInBytes, &offset);
	//context->IASetIndexBuffer(IBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	int bufferNumber = 0;
	ID3D11Buffer* buffs[2] = { ShaderFactory::ConstMatrixBuffers[ShaderFactory::CB_projection], ShaderFactory::ConstMatrixBuffers[ShaderFactory::CB_frame] };

	context->VSSetConstantBuffers(bufferNumber, 2, buffs);
	size_t index = 0;
	bufferNumber = 3;
	//for (UINT i = 0; i < (UINT)textures.size(); i++)
	//ID3D11ShaderResourceView* tex = m_model->GetMesh(c)->GetTexture(index);
	context->PSSetShaderResources(0, 1, &textures[0]);// &textures[i]);
	//context->VSSetConstantBuffers(bufferNumber, 1, &ShaderFactory::ConstFogLightBuffers[ShaderFactory::CB_fog]);
	context->IASetInputLayout(m_inputLayout);
	context->VSSetShader(m_vertexShader, NULL, 0);
	context->PSSetShader(m_pixelShader, NULL, 0);
	context->PSSetSamplers(0, 1, &m_sampler);
	context->DrawIndexedInstanced(6, m_VisibleCount, 0, 0, 0);
	return S_OK;
}
