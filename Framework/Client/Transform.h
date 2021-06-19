#pragma once
#include "Component.h"
class CTransform : public CComponent
{
public:
	enum VECTOR { SCALE, ROT, POS, REV, PARENT, END };
	enum STATE { RIGHT, UP, LOOK, POSITION };

public:
	explicit CTransform();
	virtual ~CTransform();

public:
	XMVECTOR GetState(STATE e) { return m_matWorld.r[e]; }
	VOID SetState(STATE e, XMVECTOR v) { m_matWorld.r[e] = v; }
	XMVECTOR GetScale(STATE e) { return XMVector3Length(m_matWorld.r[e]); }
	XMMATRIX& GetWorld() { return m_matWorld; }

public:
	VOID Move_Forward(const FLOAT t);
	VOID Move_Right(const FLOAT t);
	VOID Move_Up(const FLOAT t);
	VOID Move_Direction(XMVECTOR v, const FLOAT t);
	VOID Move_Target(XMVECTOR vDstPos, const FLOAT t);

public:
	VOID Rotate_Axis(XMVECTOR v, const FLOAT t);
	VOID Rotate_LookAt(XMVECTOR vTarget);

public:
	VOID Set_Scale(XMFLOAT3 v);

private:
	FLOAT m_fSpeed = 100.f;
	FLOAT m_fRotationSpeed = 1.f;
	XMFLOAT3 m_vScale = { 1.f, 1.f, 1.f };
	XMMATRIX m_matWorld = XMMatrixIdentity();

public:
	virtual HRESULT Release();
	static CTransform* Create();
};

