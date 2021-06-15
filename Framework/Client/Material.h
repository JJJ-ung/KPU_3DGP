#pragma once

class CMaterial
{
public:
	CMaterial() {}
	~CMaterial() {}

public:
	string name;
	INT illum;
	string map_Kd = "";
	FLOAT Ns;
	FLOAT Ni;
	FLOAT d;
	FLOAT Ka[3];
	FLOAT Kd[3];
	FLOAT Ks[3];
	FLOAT Tf[3];
//	CTexture* texture;
};

