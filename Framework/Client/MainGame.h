#pragma once

class CInputMgr;
class CDeviceMgr;
class CShaderMgr;
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
	HRESULT Ready_Manager();
	HRESULT Ready_Device();
	HRESULT Ready_Shader();

private:
	CDeviceMgr* m_pDeviceMgr = nullptr;
	CShaderMgr* m_pShaderMgr = nullptr;

private:
	VOID Release();

public:
	static CMainGame* Create();
};

