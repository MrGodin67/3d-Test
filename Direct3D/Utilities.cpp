#include "IncludePCH.h"
#include "Utilities.h"
#include "Locator.h"
using namespace DirectX;

namespace Utilities
{

	BoundingBox::BoundingBox()
	{
		minPt = { FLT_MAX,FLT_MAX,FLT_MAX };
		maxPt = { -FLT_MAX,-FLT_MAX,-FLT_MAX };
	}

	BoundingBox::BoundingBox(DirectX::XMFLOAT3 center, DirectX::XMFLOAT3 minPt, DirectX::XMFLOAT3 maxPt)
		:center(center), minPt(minPt), maxPt(maxPt)
	{
	}

	BoundingBox BoundingBox::TransformCoords(DirectX::XMMATRIX world)
	{

		BoundingBox box;
		XMStoreFloat3(&box.center, XMVector3TransformCoord(XMLoadFloat3(&center), world));
		XMStoreFloat3(&box.minPt, XMVector3TransformCoord(XMLoadFloat3(&minPt), world));
		XMStoreFloat3(&box.maxPt, XMVector3TransformCoord(XMLoadFloat3(&maxPt), world));
		return box;
	}
	Utilities::Plane::Plane()
	{
	}

	Utilities::Plane::Plane(DirectX::XMFLOAT3 p0, DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2)
		:pt0(p0), pt1(p1), pt2(p2)
	{
		box.minPt.x = std::min<float>(std::min<float>(pt0.x, pt1.x), pt2.x);
		box.minPt.y = std::min<float>(std::min<float>(pt0.y, pt1.y), pt2.y);
		box.minPt.z = std::min<float>(std::min<float>(pt0.z, pt1.z), pt2.z);

		box.maxPt.x = std::max<float>(std::max<float>(pt0.x, pt1.x), pt2.x);
		box.maxPt.y = std::max<float>(std::max<float>(pt0.y, pt1.y), pt2.y);
		box.maxPt.z = std::max<float>(std::max<float>(pt0.z, pt1.z), pt2.z);
		DirectX::XMVECTOR center = DirectX::XMLoadFloat3(&pt0) - DirectX::XMLoadFloat3(&pt1) -
			DirectX::XMLoadFloat3(&pt2) / 3.0f;
		DirectX::XMStoreFloat3(&box.center, center);
	}

	void Utilities::Plane::MakeNormal()
	{
		XMVECTOR p1 = XMLoadFloat3(&pt1) - XMLoadFloat3(&pt0);
		XMVECTOR p2 = XMLoadFloat3(&pt2) - XMLoadFloat3(&pt0);
		XMVECTOR norm;
		norm = XMVector3Cross(p1, p2);
		norm = XMVector3Normalize(norm);
		XMStoreFloat3(&normal, norm);
	}
	Sphere::Sphere()
	{
	}
	Sphere::Sphere(DirectX::XMFLOAT3 center, float radius)
		:center(center),radius(radius)
	{
	}
	Utilities::LineSegment::LineSegment(DirectX::XMFLOAT3 p0, DirectX::XMFLOAT3 p1)
		: pt0(p0), pt1(p1)
	{}

	Utilities::DebugTri::~DebugTri()
	{
		SafeDelete(triModel);
		SafeDelete(boxModel);
	}

	Utilities::DebugTri::DebugTri(Plane & p, BoundingBox & b)
		:box(b),plane(p)
	{
		XMFLOAT3 pt0;
		XMFLOAT3 pt1;
		XMFLOAT3 pt2;
		XMFLOAT3 pt3;
		XMFLOAT3 pt4;
		XMFLOAT3 pt5;
		XMFLOAT3 pt6;
		XMFLOAT3 pt7;
		float w = box.maxPt.x - box.minPt.x;
		float h = box.maxPt.y - box.minPt.y;
		float d = box.maxPt.z - box.minPt.z;
		int index = 0;
		float m_minWidth, m_minHeight, m_minDepth;
		float m_maxWidth, m_maxHeight, m_maxDepth;
		m_minWidth = box.minPt.x; m_minHeight = box.minPt.y; m_minDepth = box.minPt.z;
		m_maxWidth = box.maxPt.x; m_maxHeight = box.maxPt.y; m_maxDepth = box.maxPt.z;
		std::vector<Geometry::ColorVertex> vertices;
		DirectX::XMFLOAT4 lineColor(1.0f, 1.0f, 1.0f, 1.0f);
		vertices.resize(24);
		vertices[index].position = DirectX::XMFLOAT3(m_minWidth, m_minHeight, m_minDepth);
		vertices[index].color = lineColor;
		
		index++;

		vertices[index].position = XMFLOAT3(m_maxWidth, m_minHeight, m_minDepth);
		vertices[index].color = lineColor;
		
		index++;

		vertices[index].position = XMFLOAT3(m_minWidth, m_minHeight, m_maxDepth);
		vertices[index].color = lineColor;
		
		index++;

		vertices[index].position = XMFLOAT3(m_maxWidth, m_minHeight, m_maxDepth);
		vertices[index].color = lineColor;
		
		index++;

		vertices[index].position = XMFLOAT3(m_minWidth, m_minHeight, m_minDepth);
		vertices[index].color = lineColor;
		
		index++;

		vertices[index].position = XMFLOAT3(m_minWidth, m_minHeight, m_maxDepth);
		vertices[index].color = lineColor;
		
		index++;

		vertices[index].position = XMFLOAT3(m_maxWidth, m_minHeight, m_minDepth);
		vertices[index].color = lineColor;
	
		index++;

		vertices[index].position = XMFLOAT3(m_maxWidth, m_minHeight, m_maxDepth);
		vertices[index].color = lineColor;
		
		index++;

		vertices[index].position = XMFLOAT3(m_minWidth, m_maxHeight, m_minDepth);
		vertices[index].color = lineColor;
		
		index++;

		vertices[index].position = XMFLOAT3(m_maxWidth, m_maxHeight, m_minDepth);
		vertices[index].color = lineColor;
		
		index++;

		vertices[index].position = XMFLOAT3(m_minWidth, m_maxHeight, m_maxDepth);
		vertices[index].color = lineColor;
		
		index++;

		vertices[index].position = XMFLOAT3(m_maxWidth, m_maxHeight, m_maxDepth);
		vertices[index].color = lineColor;
	
		index++;

		vertices[index].position = XMFLOAT3(m_minWidth, m_maxHeight, m_minDepth);
		vertices[index].color = lineColor;
	
		index++;

		vertices[index].position = XMFLOAT3(m_minWidth, m_maxHeight, m_maxDepth);
		vertices[index].color = lineColor;
	
		index++;

		vertices[index].position = XMFLOAT3(m_maxWidth, m_maxHeight, m_minDepth);
		vertices[index].color = lineColor;
		
		index++;

		vertices[index].position = XMFLOAT3(m_maxWidth, m_maxHeight, m_maxDepth);
		vertices[index].color = lineColor;
		
		index++;

		// 4 Verticle lines.
		vertices[index].position = XMFLOAT3(m_maxWidth, m_maxHeight, m_maxDepth);
		vertices[index].color = lineColor;
		
		index++;

		vertices[index].position = XMFLOAT3(m_maxWidth, m_minHeight, m_maxDepth);
		vertices[index].color = lineColor;
		
		index++;

		vertices[index].position = XMFLOAT3(m_minWidth, m_maxHeight, m_maxDepth);
		vertices[index].color = lineColor;
		
		index++;

		vertices[index].position = XMFLOAT3(m_minWidth, m_minHeight, m_maxDepth);
		vertices[index].color = lineColor;
		
		index++;

		vertices[index].position = XMFLOAT3(m_maxWidth, m_maxHeight, m_minDepth);
		vertices[index].color = lineColor;
	
		index++;

		vertices[index].position = XMFLOAT3(m_maxWidth, m_minHeight, m_minDepth);
		vertices[index].color = lineColor;
		
		index++;

		vertices[index].position = XMFLOAT3(m_minWidth, m_maxHeight, m_minDepth);
		vertices[index].color = lineColor;
	
		index++;

		vertices[index].position = XMFLOAT3(m_minWidth, m_minHeight, m_minDepth);
		vertices[index].color = lineColor;
		

		
	
		boxModel = new LineModel(vertices);
		vertices.clear();
		vertices.emplace_back(plane.pt0, DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
		vertices.emplace_back(plane.pt1, DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
		vertices.emplace_back(plane.pt1, DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
		vertices.emplace_back(plane.pt2, DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
		vertices.emplace_back(plane.pt2, DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
		vertices.emplace_back(plane.pt0, DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
		triModel = new LineModel(vertices);
		trans = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	}
	LineModel * Utilities::DebugTri::GetTriModel()
	{
		return triModel;
	}
	LineModel * Utilities::DebugTri::GetBoxModel()
	{
		return boxModel;
	}
	void DebugTri::Rasterize(ShaderFactory::Shader * shader, ID3D11ShaderResourceView* t)
	{
		
		std::vector<ID3D11ShaderResourceView*> text;
		text.push_back(t);
		Locator::GetContext()->UpdateSubresource(ShaderFactory::ConstMatrixBuffers[ShaderFactory::CB_object], 0, nullptr, &XMMatrixTranspose(DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f)), 0, 0);
		shader->Render(Locator::GetContext(), triModel->GetMesh()->GetVertexBuffer(),
			triModel->GetMesh()->GetIndexBuffer(), triModel->GetMesh()->VertexBytes(),
			triModel->GetMesh()->IndicesCount(), text);
		
		shader->Render(Locator::GetContext(), boxModel->GetMesh()->GetVertexBuffer(),
			boxModel->GetMesh()->GetIndexBuffer(), boxModel->GetMesh()->VertexBytes(),
			boxModel->GetMesh()->IndicesCount(), text);

	}
}
