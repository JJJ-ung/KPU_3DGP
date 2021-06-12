#pragma once

class CInputMgr;
class CDeviceMgr;
class CMainGame
{
public:
	CMainGame();
	~CMainGame();

public:
	HRESULT Initialize();
	INT Update(const float& fTimeDelta);
	INT Render();

private:
	CInputMgr* m_pInputMgr = nullptr;
	CDeviceMgr* m_pDeviceMgr = nullptr;

private:
	VOID Release();

public:
	static CMainGame* Create();
};

