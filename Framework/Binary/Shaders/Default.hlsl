Texture2D gDiffuse : register(t0);
SamplerState gLinear  : register(s0);

cbuffer ObjInfo : register(b0)
{
	float4 gColor;
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

	float4 vPos = mul(float4(vin.vp, 1.f), gWorld);
	vPos = mul(vPos, gView);
	vPos = mul(vPos, gProj);
	vout.Pos = vPos;

	vout.TexCoord = vin.vt;

	return vout;
}

float4 PS(VS_OUT pin) : SV_TARGET
{
	pin.TexCoord.y = (1.f - pin.TexCoord.y);
	return gDiffuse.Sample(gLinear, pin.TexCoord);
}
