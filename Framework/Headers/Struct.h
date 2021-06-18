#pragma once

typedef struct tagObjInfo
{
	XMFLOAT4 Color;
	XMFLOAT4X4 World;
	XMFLOAT4X4 View;
	XMFLOAT4X4 Proj;
}OBJINFO;

typedef struct tagLightInfo
{
	XMFLOAT4 CameraPos;
	XMFLOAT4 LightDir[3];
	XMFLOAT4 LightDiffuse[3];
	XMFLOAT4 LightAmbient[3];
	XMFLOAT4 LightSpecular[3];
}LIGHTINFO;