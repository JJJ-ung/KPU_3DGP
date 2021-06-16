//***************************************************************************************
// color.hlsl by Frank Luna (C) 2015 All Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************

cbuffer cbPerObject : register(b0)
{
	float4 gColor;
	float4 gColor2;
};

struct VertexIn
{
	float3 PosL  : POSITION;
};

struct VertexOut
{
	float4 PosH  : SV_POSITION;
	float4 Color : COLOR;
};

static float4 p[3] =
{
	float4(0, 0.5, 0.5, 1),
	float4(0.5, -0.5, 0.5, 1),
	float4(-0.5, -0.5, 0.5, 1)
};

VertexOut VS(VertexIn vin, uint i : SV_VertexID)
{
	VertexOut vout;

	// Transform to homogeneous clip space.
	//vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);

	// Just pass vertex color into the pixel shader.
	//vout.PosH = p[i];
	vout.Color = gColor2;
	if (i == 0) vout.PosH = float4(0.0, 0.5, 0.5, 1.0);
	else if (i == 1) vout.PosH = float4(0.5, -0.5, 0.5, 1.0);
	else if (i == 2) vout.PosH = float4(-0.5, -0.5, 0.5, 1.0);

	return vout;
}

float4 PS(VertexOut pin) : SV_TARGET
{
	return pin.Color;
}


