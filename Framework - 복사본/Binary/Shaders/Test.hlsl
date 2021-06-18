//***************************************************************************************
// color.hlsl by Frank Luna (C) 2015 All Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************

cbuffer cbPerObject : register(b0)
{
	float4x4 gWorldViewProj;
	float4 gColor;
	float4 gColor2;
};

struct VertexIn
{
	float3 vp  : POSITION;
	//float2 vt : TEXCOORD;
	//float3 vn : NORMAL;
};

struct VertexOut
{
	float4 Pos  : SV_POSITION;
	float4 Color : COLOR;
};

VertexOut VS(VertexIn vin, uint i : SV_VertexID)
{
	VertexOut vout;

	// Transform to homogeneous clip space.
	vout.Pos = mul(float4(vin.vp, 1.0f), gWorldViewProj);
	//if (i == 0) vout.Pos = mul(float4(0.0, 1, 1, 1.0), gWorldViewProj);
	//else if (i == 1) vout.Pos = mul(float4(1, -1, 1, 1.0), gWorldViewProj);
	//else if (i == 2) vout.Pos = mul(float4(-1, -1, 1, 1.0), gWorldViewProj);

	// Just pass vertex color into the pixel shader.
	vout.Color = float4(vin.vp, 1.0f);

	return vout;
}

float4 PS(VertexOut pin) : SV_TARGET
{
	return pin.Color;
}


