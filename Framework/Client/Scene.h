#pragma once
#include "LightMgr.h"
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
	CLightMgr* m_pLightMgr = nullptr;
	CInputMgr* m_pInputMgr = nullptr;
	CGameMgr* m_pGameMgr = nullptr;
	CDeviceMgr* m_pDeviceMgr = nullptr;

public:
	virtual VOID Release();
};

