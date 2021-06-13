#pragma once
#include "Component.h"

class CShape : public CComponent
{
public:
	typedef struct tagVertex
	{
	public:
		tagVertex() {}
		tagVertex(float x, float y, float z, float r = 0.f, float g = 0.f, float b = 0.f, float a = 1.f)
			:vPos(x, y, z), vColor(r, g, b, a)
		{}
		tagVertex(float x, float y, float z, VEC4 c)
			:vPos(x, y, z), vColor(c)
		{}
		tagVertex(VEC3 v, VEC4 c = VEC4(0.f, 0.f, 0.f, 1.f))
			:vPos(v), vColor(c)
		{}
		VEC3 vPos;
		VEC4 vColor;
	}VERTEX;

	typedef struct tagPolygon
	{
		vector<VERTEX> vVertices;
	}POLYGON;

	enum MESH_TYPE { TRIANGLE, BOX, SPHERE, CYLINDER, GRID, QUAD, END };

	typedef struct tagInputData
	{
		MESH_TYPE eType;
		float fWidth;
		float fHeight;
		float fDepth;
		float fRadius;
		float fBottomRadius;
		float fTopRadius;
		float x;
		float y;
		UINT iSubdivisions;
		UINT iSliceCnt;
		UINT iStackCnt;
		UINT m;
		UINT n;
	}INPUT_DATA;

public:
	explicit CShape();
	virtual ~CShape();

public:
	virtual HRESULT Initialize(INPUT_DATA tData, VEC4 vColor);
	virtual INT Update(const FLOAT& fTimeDelta);
	virtual VOID Render();

public:
	VOID Set_Matrix(MAT4& matWorld, MAT4 matProj);

public:
	HRESULT Create_Triangle();
	HRESULT Create_Box(float fWidth, float fHeight, float fDepth, UINT iSubdivisions);
	HRESULT Create_Sphere(float fRadius, UINT iSliceCnt, UINT iStackCnt);
	HRESULT Create_Cylinder(float fBottomRadius, float fTopRadius, float fHeight, UINT iSliceCnt, UINT iStackCnt);
	HRESULT Create_Grid(float fWidth, float fDepth, UINT m, UINT n);
	HRESULT Create_Quad(float x, float y, float w, float h, float fDepth);

private:
	void Subdivide(POLYGON& tPolygon);
	VERTEX MidPoint(const VERTEX& v0, const VERTEX v1);
	void BuildCylinderTopCap(float fBottomRadius, float fTopRadius, float fHeight, UINT sliceCount, UINT iStackCnt, POLYGON& tPolygon);
	void BuildCylinderBottomCap(float fBottomRadius, float fTopRadius, float fHeight, UINT sliceCount, UINT iStackCnt, POLYGON& tPolygon);

private:
	POLYGON m_tMesh;
	VEC4 m_vColor = VEC4(0.f, 0.f, 0.f, 1.f);

private:
	MAT4 m_matProj;
	MAT4 m_matWorld;

public:
	virtual HRESULT Release();
	static CShape* Create(INPUT_DATA tData, VEC4 vColor);
};

