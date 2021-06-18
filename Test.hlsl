//***************************************************************************************
// color.hlsl by Frank Luna (C) 2015 All Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************

Texture2D gDiffuse : register(t0);
SamplerState gLinear  : register(s0);

cbuffer ObjInfo : register(b0)
{
	float4x4 gWorld;
	float4x4 gView;
	float4x4 gProj;
};

cbuffer LightInfo : register(b1)
{
	float4 gCameraPos;
	float4 gLightDir[3];
	float4 gLightDiffuse[3];
	float4 gLightAmbient[3];
	float4 gLightSpecular[3];
};

struct VS_IN
{
	float3 vp  : POSITION;
	float2 vt : TEXCOORD;
	float3 vn : NORMAL;
};

struct VS_OUT
{
	float4 Pos  : SV_POSITION;
	float2 TexCoord : TEXCOORD;
};

VS_OUT VS(VS_IN vin)
{
	VS_OUT vout;

	// Transform to homogeneous clip space.
	vout.Pos = mul(float4(vin.vp, 1.0f), gWorldViewProj);

	// Just pass vertex color into the pixel shader.
	//vout.Color = float4(vin.vn, 1.f);
	vout.TexCoord = vin.vt;

	return vout;
}

float4 PS(VS_OUT pin) : SV_TARGET
{
	//return float4(0.f, 0.f, 0.f, 1.f);
	pin.TexCoord.y = (1.f - pin.TexCoord.y);
	return gDiffuse.Sample(gLinear, pin.TexCoord);
}


