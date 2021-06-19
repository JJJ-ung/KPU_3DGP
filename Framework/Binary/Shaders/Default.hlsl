Texture2D gDiffuse : register(t0);
SamplerState gLinear  : register(s0);

cbuffer ObjInfo : register(b0)
{
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

cbuffer ConstInfo : register(b2)
{
	float4 gColor;
	float4x4 gWorld;
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
	float4 Normal : NORMAL;
};

VS_OUT VS(VS_IN vin)
{
	VS_OUT vout;

	float4 vPos = mul(float4(vin.vp, 1.f), gWorld);
	vPos = mul(vPos, gView);
	vPos = mul(vPos, gProj);
	vout.Pos = vPos;

	vout.TexCoord = vin.vt;
	vout.Normal = float4(vin.vn, 0.f);

	return vout;
}

float4 PS(VS_OUT pin) : SV_TARGET
{
	pin.TexCoord.y = (1.f - pin.TexCoord.y);

	float4 vColor = gDiffuse.Sample(gLinear, pin.TexCoord);
	float4 vLook = gCameraPos - pin.Pos;

	for (int i = 0; i < 3; ++i)
	{
		float4 vShade = max(dot(normalize(gLightDir[i]) * -1.f, normalize(pin.Normal)), 0.f) * gLightDiffuse[i] + gLightAmbient[i];
		float4 vReflect = reflect(normalize(gLightDir[i]), normalize(pin.Normal));
		float4 vSpecular = pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), 20.f) * gLightDiffuse[i] * gLightSpecular[i];

		vColor += vColor * vShade + vSpecular;
	}

	return vColor;
}
