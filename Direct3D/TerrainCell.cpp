#include "IncludePCH.h"
#include "TerrainCell.h"


TerrainCell::TerrainCell(ID3D11Device* device, void* terrainModelPtr, int nodeIndexX, int nodeIndexY,
	int cellHeight, int cellWidth, int terrainWidth)
{
	bool result;
	result = Initialize(device, terrainModelPtr, nodeIndexX, nodeIndexY,
		cellHeight, cellWidth, terrainWidth);
	assert(result == true);// throw assersion if false
}


TerrainCell::TerrainCell(const TerrainCell& other)
{
}


TerrainCell::~TerrainCell()
{
}


bool TerrainCell::Initialize(ID3D11Device* device, void* terrainModelPtr, int nodeIndexX, int nodeIndexY,
	int cellHeight, int cellWidth, int terrainWidth)
{
	ModelType* terrainModel;
	bool result;


	// Coerce the pointer to the terrain model into the model type.
	terrainModel = (ModelType*)terrainModelPtr;

	// Load the rendering buffers with the terrain data for this cell index.
	result = InitializeBuffers(device, nodeIndexX, nodeIndexY, cellHeight, cellWidth, terrainWidth, terrainModel);
	if (!result)
	{
		return false;
	}

	// Release the pointer to the terrain model now that we no longer need it.
	terrainModel = 0;

	// Calculuate the dimensions of this cell.
	CalculateCellDimensions();

	// Build the debug line buffers to produce the bounding box around this cell.
	result = BuildLineBuffers(device);
	if (!result)
	{
		return false;
	}

	return true;
}


void TerrainCell::Shutdown()
{
	// Release the line rendering buffers.
	ShutdownLineBuffers();

	// Release the cell rendering buffers.
	ShutdownBuffers();

	return;
}


void TerrainCell::Render(ID3D11DeviceContext* deviceContext)
{
	
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}


int TerrainCell::GetVertexCount()
{
	return m_vertexCount;
}


int TerrainCell::GetIndexCount()
{
	return m_indexCount;
}


bool TerrainCell::InitializeBuffers(ID3D11Device* device, int nodeIndexX, int nodeIndexY, int cellHeight, int cellWidth,
	int terrainWidth, ModelType* terrainModel)
{
	VertexType* vertices;
	unsigned long* indices;
	int i, j, modelIndex, index;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;


	// Calculate the number of vertices in this terrain cell.
	m_vertexCount = (cellHeight - 1) * (cellWidth - 1) * 6;

	// Set the index count to the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// Setup the indexes into the terrain model data and the local vertex/index array.
	modelIndex = ((nodeIndexX * (cellWidth - 1)) + (nodeIndexY * (cellHeight - 1) * (terrainWidth - 1))) * 6;
	index = 0;

	// Load the vertex array and index array with data.
	for (j = 0; j<(cellHeight - 1); j++)
	{
		for (i = 0; i<((cellWidth - 1) * 6); i++)
		{
			vertices[index].position = XMFLOAT3(terrainModel[modelIndex].x, terrainModel[modelIndex].y, terrainModel[modelIndex].z);
			vertices[index].texture = XMFLOAT2(terrainModel[modelIndex].tu, terrainModel[modelIndex].tv);
			vertices[index].normal = XMFLOAT3(terrainModel[modelIndex].nx, terrainModel[modelIndex].ny, terrainModel[modelIndex].nz);
			vertices[index].tangent = XMFLOAT3(terrainModel[modelIndex].tx, terrainModel[modelIndex].ty, terrainModel[modelIndex].tz);
			vertices[index].binormal = XMFLOAT3(terrainModel[modelIndex].bx, terrainModel[modelIndex].by, terrainModel[modelIndex].bz);
			vertices[index].color = XMFLOAT3(terrainModel[modelIndex].r, terrainModel[modelIndex].g, terrainModel[modelIndex].b);
			indices[index] = index;
			modelIndex++;
			index++;
		}
		modelIndex += (terrainWidth * 6) - (cellWidth * 6);
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Create a public vertex array that will be used for accessing vertex information about this cell.
	m_vertexList = new VectorType[m_vertexCount];
	if (!m_vertexList)
	{
		return false;
	}

	// Keep a local copy of the vertex position data for this cell.
	for (i = 0; i<m_vertexCount; i++)
	{
		m_vertexList[i].x = vertices[i].position.x;
		m_vertexList[i].y = vertices[i].position.y;
		m_vertexList[i].z = vertices[i].position.z;
	}

	// Release the arrays now that the buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}


void TerrainCell::ShutdownBuffers()
{
	
	SafeDeleteArray(m_vertexList);
	SafeRelease(m_indexBuffer);
	SafeRelease(m_vertexBuffer);
	

	return;
}





void TerrainCell::CalculateCellDimensions()
{
	int i;
	float width, height, depth;


	// Initialize the dimensions of the node.
	XMFLOAT3 m_Max = XMFLOAT3(-1000000.0f, -1000000.0f, -1000000.0f);
	XMFLOAT3 m_Min = XMFLOAT3(1000000.0f, 1000000.0f, 1000000.0f);
	
	for (i = 0; i<m_vertexCount; i++)
	{
		width = m_vertexList[i].x;
		height = m_vertexList[i].y;
		depth = m_vertexList[i].z;

		// Check if the width exceeds the minimum or maximum.
		m_Max.x =__max(width, m_Max.x);
		m_Max.y = __max(height, m_Max.y);
		m_Max.z = __max(depth, m_Max.z);

		m_Min.x = __min(width, m_Min.x);
		m_Min.y = __min(height, m_Min.y);
		m_Min.z = __min(depth, m_Min.z);
		
	}
	m_Max.y += 10.0f;
	m_Min.y -= 10.0f;
	// Calculate the center position of this cell.
	m_Center.x = (m_Max.x - m_Min.x) + m_Min.x;
	m_Center.y = (m_Max.y - m_Min.y) + m_Min.y;
	m_Center.z = (m_Max.z - m_Min.z) + m_Min.z;

	m_BoundingBox = Utilities::BoundingBox(m_Center,m_Min,m_Max);

	
}


bool TerrainCell::BuildLineBuffers(ID3D11Device* device)
{
	ColorVertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	XMFLOAT4 lineColor;
	int index, vertexCount, indexCount;


	// Set the color of the lines to orange.
	lineColor = XMFLOAT4(1.0f, 0.5f, 0.0f, 1.0f);

	// Set the number of vertices in the vertex array.
	vertexCount = 24;

	// Set the number of indices in the index array.
	indexCount = vertexCount;

	// Create the vertex array.
	vertices = new ColorVertexType[vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[indexCount];
	if (!indices)
	{
		return false;
	}

	// Set up the description of the vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(ColorVertexType) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Set up the description of the index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Load the vertex and index array with data.
	index = 0;

	// 8 Horizontal lines.
	vertices[index].position = XMFLOAT3(m_minWidth, m_minHeight, m_minDepth);
	vertices[index].color = lineColor;
	indices[index] = index;
	index++;

	vertices[index].position = XMFLOAT3(m_maxWidth, m_minHeight, m_minDepth);
	vertices[index].color = lineColor;
	indices[index] = index;
	index++;

	vertices[index].position = XMFLOAT3(m_minWidth, m_minHeight, m_maxDepth);
	vertices[index].color = lineColor;
	indices[index] = index;
	index++;

	vertices[index].position = XMFLOAT3(m_maxWidth, m_minHeight, m_maxDepth);
	vertices[index].color = lineColor;
	indices[index] = index;
	index++;

	vertices[index].position = XMFLOAT3(m_minWidth, m_minHeight, m_minDepth);
	vertices[index].color = lineColor;
	indices[index] = index;
	index++;

	vertices[index].position = XMFLOAT3(m_minWidth, m_minHeight, m_maxDepth);
	vertices[index].color = lineColor;
	indices[index] = index;
	index++;

	vertices[index].position = XMFLOAT3(m_maxWidth, m_minHeight, m_minDepth);
	vertices[index].color = lineColor;
	indices[index] = index;
	index++;

	vertices[index].position = XMFLOAT3(m_maxWidth, m_minHeight, m_maxDepth);
	vertices[index].color = lineColor;
	indices[index] = index;
	index++;

	vertices[index].position = XMFLOAT3(m_minWidth, m_maxHeight, m_minDepth);
	vertices[index].color = lineColor;
	indices[index] = index;
	index++;

	vertices[index].position = XMFLOAT3(m_maxWidth, m_maxHeight, m_minDepth);
	vertices[index].color = lineColor;
	indices[index] = index;
	index++;

	vertices[index].position = XMFLOAT3(m_minWidth, m_maxHeight, m_maxDepth);
	vertices[index].color = lineColor;
	indices[index] = index;
	index++;

	vertices[index].position = XMFLOAT3(m_maxWidth, m_maxHeight, m_maxDepth);
	vertices[index].color = lineColor;
	indices[index] = index;
	index++;

	vertices[index].position = XMFLOAT3(m_minWidth, m_maxHeight, m_minDepth);
	vertices[index].color = lineColor;
	indices[index] = index;
	index++;

	vertices[index].position = XMFLOAT3(m_minWidth, m_maxHeight, m_maxDepth);
	vertices[index].color = lineColor;
	indices[index] = index;
	index++;

	vertices[index].position = XMFLOAT3(m_maxWidth, m_maxHeight, m_minDepth);
	vertices[index].color = lineColor;
	indices[index] = index;
	index++;

	vertices[index].position = XMFLOAT3(m_maxWidth, m_maxHeight, m_maxDepth);
	vertices[index].color = lineColor;
	indices[index] = index;
	index++;

	// 4 Verticle lines.
	vertices[index].position = XMFLOAT3(m_maxWidth, m_maxHeight, m_maxDepth);
	vertices[index].color = lineColor;
	indices[index] = index;
	index++;

	vertices[index].position = XMFLOAT3(m_maxWidth, m_minHeight, m_maxDepth);
	vertices[index].color = lineColor;
	indices[index] = index;
	index++;

	vertices[index].position = XMFLOAT3(m_minWidth, m_maxHeight, m_maxDepth);
	vertices[index].color = lineColor;
	indices[index] = index;
	index++;

	vertices[index].position = XMFLOAT3(m_minWidth, m_minHeight, m_maxDepth);
	vertices[index].color = lineColor;
	indices[index] = index;
	index++;

	vertices[index].position = XMFLOAT3(m_maxWidth, m_maxHeight, m_minDepth);
	vertices[index].color = lineColor;
	indices[index] = index;
	index++;

	vertices[index].position = XMFLOAT3(m_maxWidth, m_minHeight, m_minDepth);
	vertices[index].color = lineColor;
	indices[index] = index;
	index++;

	vertices[index].position = XMFLOAT3(m_minWidth, m_maxHeight, m_minDepth);
	vertices[index].color = lineColor;
	indices[index] = index;
	index++;

	vertices[index].position = XMFLOAT3(m_minWidth, m_minHeight, m_minDepth);
	vertices[index].color = lineColor;
	indices[index] = index;

	// Create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_lineVertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_lineIndexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Store the index count for rendering.
	m_lineIndexCount = indexCount;

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}


void TerrainCell::ShutdownLineBuffers()
{
	SafeRelease(m_lineIndexBuffer);
	SafeRelease(m_lineVertexBuffer);
}


void TerrainCell::RenderLineBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(ColorVertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_lineVertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_lineIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case lines.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	return;
}


int TerrainCell::GetLineBuffersIndexCount()
{
	return m_lineIndexCount;
}

Utilities::BoundingBox TerrainCell::BoundingBox()
{
	return m_BoundingBox;
}


