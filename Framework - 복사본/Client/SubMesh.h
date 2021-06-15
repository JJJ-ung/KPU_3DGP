#pragma once

struct Vertex
{
public:
	Vertex() {}
	Vertex(VEC3 vp, VEC2 vt, VEC3 vn)
		: v(vp), uv(vt), n(vn)	{}
	VEC3 v;
	VEC2 uv;
	VEC3 n;
};

struct Face
{
public:
	Face() {}
	Face(UINT vp, UINT vt, UINT vn)
		: v(vp), uv(vt), n(vn) {}
	UINT v;
	UINT uv;
	UINT n;
};

class CMaterial;
class CSubMesh
{
public:
	CSubMesh() {}
	~CSubMesh() {}

public:
	vector<Vertex> vertices;
	vector<Face> indices;
	CMaterial* material = nullptr;
};
