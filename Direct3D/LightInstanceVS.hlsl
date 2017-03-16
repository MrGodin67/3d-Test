

cbuffer MatrixBuffer
{
	matrix projectionMatrix;
};
cbuffer PerFrame
{
	matrix viewMatrix;
};
cbuffer PerObject
{
	matrix worldMatrix;
};
cbuffer FogBuffer
{
	float fogStart;
	float fogRange;
	float padding;
	float padding2;
};

//////////////
// TYPEDEFS //
//////////////
//float3 InstancePosition : TEXCOORD1;
struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 InstancePosition : TEXCOORD1;
	

};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 InstancePosition : TEXCOORD1;
	float fogFactor : FOG;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType LightInstanceVertexShader(VertexInputType input)
{
	PixelInputType output;
	float4 cameraPosition;

	// Change the position vector to be 4 units for proper matrix calculations.
	input.position.w = 1.0f;
	input.position.x += input.InstancePosition.x;
	input.position.y += input.InstancePosition.y;
	input.position.z += input.InstancePosition.z;
	output.position = mul(input.position, (worldMatrix));//input.InstanceMatrix));
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	output.normal = input.normal; 
	//OUT.TexCoord = IN.TexCoord;
	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

	// Calculate the normal vector against the world matrix only.
	//output.normal = mul(input.normal, (float3x3)worldMatrix);

	// Normalize the normal vector.
	output.normal = normalize(output.normal);

	cameraPosition = mul(input.position, worldMatrix);
	cameraPosition = mul(cameraPosition, viewMatrix);
	float dist = distance(output.position, cameraPosition);
	output.fogFactor = saturate((dist - fogStart) / fogRange);
	return output;
}