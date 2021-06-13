#include "framework.h"
#include "Shape.h"

CShape::CShape()
{
}

CShape::~CShape()
{
	Release();
}

HRESULT CShape::Initialize(INPUT_DATA tData, VEC4 vColor)
{
	m_vColor = vColor;
	switch (tData.eType)
	{
	case TRIANGLE:
		if (FAILED(Create_Triangle()))
			return E_FAIL;
		break;
	case BOX:
		if (FAILED(Create_Box(tData.fWidth, tData.fHeight, tData.fDepth, tData.iSubdivisions)))
			return E_FAIL;
		break;
	case SPHERE:
		if (FAILED(Create_Sphere(tData.fRadius, tData.iSliceCnt, tData.iStackCnt)))
			return E_FAIL;
		break;
	case CYLINDER:
		if (FAILED(Create_Cylinder(tData.fBottomRadius, tData.fTopRadius, tData.fHeight, tData.iSliceCnt, tData.iStackCnt)))
			return E_FAIL;
		break;
	case GRID:
		if (FAILED(Create_Grid(tData.fWidth, tData.fDepth, tData.m, tData.n)))
			return E_FAIL;
		break;
	case QUAD:
		if (FAILED(Create_Quad(tData.x, tData.y, tData.fWidth, tData.fHeight, tData.fDepth)))
			return E_FAIL;
		break;
	default:
		return E_FAIL;
	}

	if (m_tMesh.vVertices.empty())
		return E_FAIL;

	return NOERROR;
}

INT CShape::Update(const FLOAT& fTimeDelta)
{
	return 0;
}

VOID CShape::Render()
{
	return VOID();
}

VOID CShape::Set_Matrix(MAT4& matWorld, MAT4 matProj)
{
	m_matProj = matProj;
	m_matWorld = matWorld;
}

HRESULT CShape::Create_Triangle()
{
	VERTEX v[3];
	v[0] = VERTEX(0.f, 0.5f, 0.f, m_vColor);
	v[1] = VERTEX(0.5f, -0.5f, 0.f, m_vColor);
	v[2] = VERTEX(-0.5f, -0.5f, 0.f, m_vColor);

	m_tMesh.vVertices.assign(&v[0], &v[3]);

	return NOERROR;
}

HRESULT CShape::Create_Box(float fWidth, float fHeight, float fDepth, UINT iSubdivisions)
{
	VERTEX v[24];
	float w2 = 0.5f * fWidth;
	float h2 = 0.5f * fHeight;
	float d2 = 0.5f * fDepth;

	v[0] = VERTEX(-w2, -h2, -d2, m_vColor);
	v[1] = VERTEX(-w2, +h2, -d2, m_vColor);
	v[2] = VERTEX(+w2, +h2, -d2, m_vColor);
	v[3] = VERTEX(+w2, -h2, -d2, m_vColor);

	v[4] = VERTEX(-w2, -h2, +d2, m_vColor);
	v[5] = VERTEX(+w2, -h2, +d2, m_vColor);
	v[6] = VERTEX(+w2, +h2, +d2, m_vColor);
	v[7] = VERTEX(-w2, +h2, +d2, m_vColor);

	v[8] = VERTEX(-w2, +h2, -d2, m_vColor);
	v[9] = VERTEX(-w2, +h2, +d2, m_vColor);
	v[10] = VERTEX(+w2, +h2, +d2, m_vColor);
	v[11] = VERTEX(+w2, +h2, -d2, m_vColor);

	v[12] = VERTEX(-w2, -h2, -d2, m_vColor);
	v[13] = VERTEX(+w2, -h2, -d2, m_vColor);
	v[14] = VERTEX(+w2, -h2, +d2, m_vColor);
	v[15] = VERTEX(-w2, -h2, +d2, m_vColor);

	v[16] = VERTEX(-w2, -h2, +d2, m_vColor);
	v[17] = VERTEX(-w2, +h2, +d2, m_vColor);
	v[18] = VERTEX(-w2, +h2, -d2, m_vColor);
	v[19] = VERTEX(-w2, -h2, -d2, m_vColor);

	v[20] = VERTEX(+w2, -h2, -d2, m_vColor);
	v[21] = VERTEX(+w2, +h2, -d2, m_vColor);
	v[22] = VERTEX(+w2, +h2, +d2, m_vColor);
	v[23] = VERTEX(+w2, -h2, +d2, m_vColor);

	m_tMesh.vVertices.assign(&v[0], &v[24]);

	iSubdivisions = min<UINT>(iSubdivisions, 6u);
	for (UINT i = 0; i < iSubdivisions; ++i)
		Subdivide(m_tMesh);

	return NOERROR;
}

HRESULT CShape::Create_Sphere(float fRadius, UINT iSliceCnt, UINT iStackCnt)
{
	VERTEX topVertex(0.0f, +fRadius, 0.0f, m_vColor);
	VERTEX bottomVertex(0.0f, -fRadius, 0.0f, m_vColor);

	m_tMesh.vVertices.push_back(topVertex);

	float phiStep = XM_PI / iStackCnt;
	float thetaStep = 2.0f * XM_PI / iSliceCnt;

	for (UINT i = 1; i <= iStackCnt - 1; ++i)
	{
		float phi = i * phiStep;

		for (UINT j = 0; j <= iSliceCnt; ++j)
		{
			float theta = j * thetaStep;

			VERTEX v;
			v.vPos.x = fRadius * sinf(phi) * cosf(theta);
			v.vPos.y = fRadius * cosf(phi);
			v.vPos.z = fRadius * sinf(phi) * sinf(theta);
			v.vColor = m_vColor;

			m_tMesh.vVertices.push_back(v);
		}
	}

	m_tMesh.vVertices.push_back(bottomVertex);

	return NOERROR;
}

HRESULT CShape::Create_Cylinder(float fBottomRadius, float fTopRadius, float fHeight, UINT iSliceCnt, UINT iStackCnt)
{
	float stackHeight = fHeight / iStackCnt;
	float radiusStep = (fTopRadius - fBottomRadius) / iStackCnt;
	UINT ringCount = iStackCnt + 1;

	for (UINT i = 0; i < ringCount; ++i)
	{
		float y = -0.5f * fHeight + i * stackHeight;
		float r = fBottomRadius + i * radiusStep;

		float dTheta = 2.0f * XM_PI / iSliceCnt;
		for (UINT j = 0; j <= iSliceCnt; ++j)
		{
			VERTEX vertex;

			float c = cosf(j * dTheta);
			float s = sinf(j * dTheta);

			vertex.vPos = XMFLOAT3(r * c, y, r * s);
			vertex.vColor = m_vColor;

			m_tMesh.vVertices.push_back(vertex);
		}
	}

	BuildCylinderTopCap(fBottomRadius, fTopRadius, fHeight, iSliceCnt, iStackCnt, m_tMesh);
	BuildCylinderBottomCap(fBottomRadius, fTopRadius, fHeight, iSliceCnt, iStackCnt, m_tMesh);

	return NOERROR;
}

HRESULT CShape::Create_Grid(float fWidth, float fDepth, UINT m, UINT n)
{
	UINT vertexCount = m * n;
	UINT faceCount = (m - 1) * (n - 1) * 2;

	float halfWidth = 0.5f * fWidth;
	float halfDepth = 0.5f * fDepth;

	float dx = fWidth / (n - 1);
	float dz = fDepth / (m - 1);

	float du = 1.0f / (n - 1);
	float dv = 1.0f / (m - 1);

	m_tMesh.vVertices.resize(vertexCount);
	for (UINT i = 0; i < m; ++i)
	{
		float z = halfDepth - i * dz;
		for (UINT j = 0; j < n; ++j)
		{
			float x = -halfWidth + j * dx;
			m_tMesh.vVertices[i * n + j].vPos = XMFLOAT3(x, 0.0f, z);
			m_tMesh.vVertices[i * n + j].vColor = m_vColor;
		}
	}

	return NOERROR;
}

HRESULT CShape::Create_Quad(float x, float y, float w, float h, float fDepth)
{
	m_tMesh.vVertices.resize(4);

	m_tMesh.vVertices[0] = VERTEX(x, y - h, fDepth, m_vColor);
	m_tMesh.vVertices[1] = VERTEX(x, y, fDepth, m_vColor);
	m_tMesh.vVertices[2] = VERTEX(x + w, y, fDepth, m_vColor);
	m_tMesh.vVertices[3] = VERTEX(x + w, y - h, fDepth, m_vColor);

	return NOERROR;
}

void CShape::Subdivide(POLYGON& tPolygon)
{
	POLYGON inputCopy = tPolygon;

	vector<UINT> tmp;
	UINT i[36];
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;
	i[6] = 4; i[7] = 5; i[8] = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;
	i[12] = 8; i[13] = 9; i[14] = 10;
	i[15] = 8; i[16] = 10; i[17] = 11;
	i[18] = 12; i[19] = 13; i[20] = 14;
	i[21] = 12; i[22] = 14; i[23] = 15;
	i[24] = 16; i[25] = 17; i[26] = 18;
	i[27] = 16; i[28] = 18; i[29] = 19;
	i[30] = 20; i[31] = 21; i[32] = 22;
	i[33] = 20; i[34] = 22; i[35] = 23;
	tmp.assign(&i[0], &i[36]);

	tPolygon.vVertices.resize(0);

	UINT numTris = (UINT)tmp.size() / 3;
	for (UINT i = 0; i < numTris; ++i)
	{
		VERTEX v0 = inputCopy.vVertices[tmp[i * 3 + 0]];
		VERTEX v1 = inputCopy.vVertices[tmp[i * 3 + 1]];
		VERTEX v2 = inputCopy.vVertices[tmp[i * 3 + 2]];

		VERTEX m0 = MidPoint(v0, v1);
		VERTEX m1 = MidPoint(v1, v2);
		VERTEX m2 = MidPoint(v0, v2);

		tPolygon.vVertices.push_back(v0); // 0
		tPolygon.vVertices.push_back(v1); // 1
		tPolygon.vVertices.push_back(v2); // 2
		tPolygon.vVertices.push_back(m0); // 3
		tPolygon.vVertices.push_back(m1); // 4
		tPolygon.vVertices.push_back(m2); // 5
	}
}

CShape::VERTEX CShape::MidPoint(const VERTEX& v0, const VERTEX v1)
{
	XMVECTOR p0 = XMLoadFloat3(&v0.vPos);
	XMVECTOR p1 = XMLoadFloat3(&v1.vPos);

	XMVECTOR pos = 0.5f * (p0 + p1);

	VERTEX v;
	XMStoreFloat3(&v.vPos, pos);

	return v;
}

void CShape::BuildCylinderTopCap(float fBottomRadius, float fTopRadius, float fHeight, UINT sliceCount, UINT iStackCnt, POLYGON& tPolygon)
{
	UINT baseIndex = (UINT)tPolygon.vVertices.size();

	float y = 0.5f * fHeight;
	float dTheta = 2.0f * XM_PI / sliceCount;

	for (UINT i = 0; i <= sliceCount; ++i)
	{
		float x = fTopRadius * cosf(i * dTheta);
		float z = fTopRadius * sinf(i * dTheta);

		float u = x / fHeight + 0.5f;
		float v = z / fHeight + 0.5f;

		tPolygon.vVertices.push_back(VERTEX(x, y, z, m_vColor));
	}

	tPolygon.vVertices.push_back(VERTEX(0.0f, y, 0.0f, m_vColor));
}

void CShape::BuildCylinderBottomCap(float fBottomRadius, float fTopRadius, float fHeight, UINT sliceCount, UINT iStackCnt, POLYGON& tPolygon)
{
	UINT baseIndex = (UINT)tPolygon.vVertices.size();
	float y = -0.5f * fHeight;

	float dTheta = 2.0f * XM_PI / sliceCount;
	for (UINT i = 0; i <= sliceCount; ++i)
	{
		float x = fBottomRadius * cosf(i * dTheta);
		float z = fBottomRadius * sinf(i * dTheta);

		float u = x / fHeight + 0.5f;
		float v = z / fHeight + 0.5f;

		tPolygon.vVertices.push_back(VERTEX(x, y, z, m_vColor));
	}

	tPolygon.vVertices.push_back(VERTEX(0.0f, y, 0.0f, m_vColor));
}

HRESULT CShape::Release()
{
	return NOERROR;
}

CShape* CShape::Create(INPUT_DATA tData, VEC4 vColor)
{
	CShape* pInstance = new CShape;
	if (FAILED(pInstance->Initialize(tData, vColor)))
		SafeDelete(pInstance);
	return pInstance;
}
