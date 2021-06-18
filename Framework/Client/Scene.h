#pragma once
#include "InputMgr.h"
#include "GameMgr.h"
#include "DeviceMgr.h"
class CScene
{
public:
	explicit CScene();
	virtual ~CScene();

public:
	virtual HRESULT Initialize();
	virtual INT Update(const FLOAT& fTimeDelta);

protected:
	CInputMgr* m_pInputMgr = nullptr;
	CGameMgr* m_pGameMgr = nullptr;
	CDeviceMgr* m_pDeviceMgr = nullptr;

public:
	virtual VOID Release();
};

