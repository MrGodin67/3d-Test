#include "IncludePCH.h"
#include "Model.h"
#include "TextureFactory.h"
using namespace std;
using namespace DirectX;

WavefrontModel::WavefrontModel()
{
}

HRESULT WavefrontModel::LoadFromFile(std::string filename)
{
	
	vector<tinyobj::shape_t> shapes;
	
	std::string str_err;
	std::string filepath;
	
	std::string fn = filename;
	size_t index = fn.find_last_of("/\\");
	filepath = fn.substr(0, index);
	filepath += "\\";
	
	bool result = tinyobj::LoadObj(shapes, m_materials, str_err, filename.c_str(), filepath.c_str());
	if (result)
	{
		int m_numbShapes = (int)shapes.size();
		int m_numbMaterials = (int)m_materials.size();
		m_textures.resize(m_materials.size());
		for (int c = 0; c < m_numbMaterials; c++)
		{
			string str = filepath;
			str += m_materials[c].diffuse_texname;
			m_materials[c].diffuse_texname = str;
			HRESULT hr;
			hr = TextureFactory::CreateSRV(&m_textures[c], str);
			assert(hr == S_OK);
		}
		std::vector<Geometry::LOD_VertexMin> verts;
		std::vector<DWORD> inds;
		float minx = FLT_MAX, miny = FLT_MAX, minz = FLT_MAX;
		float maxx = -FLT_MAX, maxy = -FLT_MAX, maxz = -FLT_MAX;
		for (int i = 0; i < m_numbShapes; i++)
		{
			tinyobj::mesh_t* mesh = &shapes[i].mesh;
			verts.clear();
			inds.clear();
			for (unsigned int j = 0; j < mesh->indices.size(); j++)
			{
				unsigned int index = j * 3;
				unsigned int tu = j * 2;
				Geometry::LOD_VertexMin pnt;
				
				pnt.pos.x = mesh->positions[index];
				pnt.pos.y = mesh->positions[index + 1];
				pnt.pos.z = mesh->positions[index + 2];
				
				// get min max vertices for aabb
				minx = __min(minx, pnt.pos.x);
				miny = __min(miny, pnt.pos.y);
				minz = __min(minz, pnt.pos.z);

				maxx = __max(maxx, pnt.pos.x);
				maxy = __max(maxy, pnt.pos.y);
				maxz = __max(maxz, pnt.pos.z);

				pnt.tex.x = mesh->texcoords[tu];
				pnt.tex.y = mesh->texcoords[tu + 1];
				
				pnt.normal.x = mesh->normals[index];
				pnt.normal.y = mesh->normals[index + 1];
				pnt.normal.z = mesh->normals[index + 2];
				
				XMStoreFloat3(&pnt.normal, XMVector3Normalize(XMLoadFloat3(&pnt.normal)));
				
				verts.push_back(pnt);
				inds.push_back((DWORD)mesh->indices[j]);
			}
			/*UINT c = 0;
			for (c = 0; c < inds.size() / 3; c++)
			{
				int index = c * 3;
				Plane p;
				p.pt0 = float3(verts[inds[index]].x, verts[inds[index]].y, verts[inds[index]].z);
				p.pt1 = float3(verts[inds[index+1]].x, verts[inds[index+1]].y, verts[inds[index+1]].z);
				p.pt2 = float3(verts[inds[index + 2]].x, verts[inds[index + 2]].y, verts[inds[index + 2]].z);
				p.MakeNormal();
				m_faces.push_back(p);
			}*/
			
			m_meshes.emplace_back(verts, inds);
			
			
		}
		m_WHD.x= maxx - minx;
		m_WHD.y = maxy - miny;
		m_WHD.z = maxz - minz;
		aabb.center = { 0.0f,0.0f,0.0f };
		aabb.minPt = XMFLOAT3(minx, miny, minz);
		aabb.maxPt = XMFLOAT3(maxx, maxy, maxz);
	}
	else
	{
		return E_FAIL;
	}
	for (int c = 0; c < m_meshes.size(); c++)
	{
		m_meshes[c].SetTexture(m_textures[c]);
	}

	return S_OK;
}


size_t WavefrontModel::MeshCount()
{
	return m_meshes.size();
}

size_t WavefrontModel::MaterialCount()
{
	return m_materials.size();;
}

string WavefrontModel::GetMaterialDiffuseName(const size_t index)
{
	return m_materials[index].diffuse_texname;
}

MeshFactory::Mesh<Geometry::LOD_VertexMin> * WavefrontModel::GetMesh(const size_t index)
{
	if (index < 0 || index >= m_meshes.size())
		return nullptr;

	return &m_meshes[index];
}



void WavefrontModel::Release()
{
	for (auto& it : m_meshes)
		it.Release();
}

float WavefrontModel::Width()
{
	return m_WHD.x;
}

float WavefrontModel::Height()
{
	return m_WHD.y;
}

float WavefrontModel::Depth()
{
	return m_WHD.z;
}

Utilities::BoundingBox WavefrontModel::BoundingBox()
{
	return aabb;
}

//Tools3D::AABB_3D WavefrontModel::BoundingBox()
//{
//	
//	return AABB_3D::Make({ 0.0f,0.0f,0.0f }, m_WHD.width, m_WHD.height, m_WHD.depth);
//}
//PlaneIntersect  WavefrontModel::RayIntersectMeshMouse( XMFLOAT2 mouse, D3D11_VIEWPORT& vp, XMMATRIX world, XMMATRIX view, XMMATRIX projection)
//{
//	float px = 0.0f;
//	float py = 0.0f;
//	PlaneIntersect intersection;
//	vector<PlaneIntersect> intersects;
//	intersection.hit = false;
//	intersection.dist[0] = 10000000.0f;
//	intersects.push_back(intersection);
//	
//	XMFLOAT4X4 matrix;
//	XMStoreFloat4x4(&matrix, projection);
//	XMFLOAT4X4 world4x4;
//	XMStoreFloat4x4(&world4x4, world);
//
//	px = (((2.0f*mouse.x) / vp.Width) - 1.0f) / matrix._11;
//	py = (((-2.0f*mouse.y) / vp.Height) + 1.0f) / matrix._22;
//
//
//	XMVECTOR origin = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
//	XMVECTOR direction = XMVectorSet(px, py, 1.0f, 1.0f);
//	XMVECTOR det =  XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
//	view = XMMatrixInverse(&det, view);
//	origin = XMVector3TransformCoord(origin, view);
//	direction = XMVector3TransformNormal(direction, view);
//	direction = XMVector3Normalize(direction);
//	
//	float3 O(XMVectorGetX(origin), XMVectorGetY(origin),XMVectorGetZ(origin));
//	float3 D(XMVectorGetX(direction), XMVectorGetY(direction), XMVectorGetZ(direction));
//	vector<Plane> faces(m_faces);
//	int x = 0;
//	std::unique(faces.begin(), faces.end(),[](Plane a, Plane b) {
//		return (a.pt0 == b.pt0 && a.pt1 == b.pt1 && a.pt2 == b.pt2);
//	});
//	for (size_t i = 0; i < m_faces.size(); i++)
//	{
//		
//		faces[i].pt0.TransformCoord(world4x4);
//		faces[i].pt1.TransformCoord(world4x4);
//		faces[i].pt2.TransformCoord(world4x4);
//		faces[i].MakeNormal();
//		PlaneIntersect p;
//		p.plane = faces[i];
//		p.line = LineSegment(O, (O + D*1000.0f));
//		Math::LineIntersectPlane(p);
//		if (p.hit)
//		{
//			intersects.push_back(p);
//		}
//	}
//	
//	std::sort(intersects.begin(),intersects.end(),[](PlaneIntersect a, PlaneIntersect b)
//	{
//		return a.dist[0] < b.dist[0];
//	});
//	
//	return intersects[0];
//}
