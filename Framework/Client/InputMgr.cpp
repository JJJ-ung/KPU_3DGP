#include "framework.h"
#include "InputMgr.h"

IMPLEMENT_SINGLETON(CInputMgr)

CInputMgr::CInputMgr()
{
}

CInputMgr::~CInputMgr()
{
}

BOOL CInputMgr::KeyDown(DWORD dwCurKey)
{
	if (!(m_dwKeyDown & dwCurKey) && (m_dwCurKey & dwCurKey))
	{
		m_dwKeyDown |= dwCurKey;
		return true;
	}

	else if ((m_dwKeyDown & dwCurKey) && !(m_dwCurKey & dwCurKey))
	{
		m_dwKeyDown ^= dwCurKey;
		return false;
	}

	return false;
}

BOOL CInputMgr::KeyUp(DWORD dwCurKey)
{
	if ((m_dwKeyUp & dwCurKey) && !(m_dwCurKey & dwCurKey))
	{
		m_dwKeyUp ^= dwCurKey;
		return true;
	}

	else if (!(m_dwKeyUp & dwCurKey) && (m_dwCurKey & dwCurKey))
	{
		m_dwKeyUp |= dwCurKey;
		return false;
	}

	return false;
}

BOOL CInputMgr::KeyPressing(DWORD dwCurKey)
{
	if (m_dwCurKey & dwCurKey)
		return true;

	return false;
}

BOOL CInputMgr::KeyCombined(DWORD dwFirstKey, DWORD dwSecondKey)
{
	if (KeyDown(dwSecondKey) && KeyPressing(dwFirstKey))
		return true;

	return false;
}

HRESULT CInputMgr::Init_InputDev()
{
	if (FAILED(DirectInput8Create(g_hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInputSDK, NULL)))
		return E_FAIL;

	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr)))
		return E_FAIL;

	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);

	m_pKeyBoard->SetCooperativeLevel(g_hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	m_pKeyBoard->Acquire();

	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		return E_FAIL;

	m_pMouse->SetDataFormat(&c_dfDIMouse);

	m_pMouse->SetCooperativeLevel(g_hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	m_pMouse->Acquire();

	return NOERROR;
}

void CInputMgr::UpdateKey()
{
	m_dwCurKey = 0;

	m_pKeyBoard->GetDeviceState(256, m_byKeyState);
	m_pMouse->GetDeviceState(sizeof(m_tMouseState), &m_tMouseState);

	if (GetKeyState(DIK_W) & 0x80)
		m_dwCurKey |= KEY_W;
	if (GetKeyState(DIK_S) & 0x80)
		m_dwCurKey |= KEY_S;
	if (GetKeyState(DIK_A) & 0x80)
		m_dwCurKey |= KEY_A;
	if (GetKeyState(DIK_D) & 0x80)
		m_dwCurKey |= KEY_D;
	if (GetKeyState(DIK_Q) & 0x80)
		m_dwCurKey |= KEY_Q;
	if (GetKeyState(DIK_E) & 0x80)
		m_dwCurKey |= KEY_E;
	if (GetKeyState(DIK_LSHIFT))
		m_dwCurKey |= KEY_SHIFT;
	if (GetKeyState(DIK_LCONTROL))
		m_dwCurKey |= KEY_CTRL;
	if (GetKeyState(DIK_SPACE))
		m_dwCurKey |= KEY_SPACE;

	if (GetMouseState(DIM_LB) & 0x80)
		m_dwCurKey |= KEY_LBUTTON;
	if (GetMouseState(DIM_RB) & 0x80)
		m_dwCurKey |= KEY_RBUTTON;

	if (GetKeyState(DIK_RETURN) & 0x80)
		m_dwCurKey |= KEY_ENTER;

	if (GetKeyState(DIK_F1) & 0x80)
		m_dwCurKey |= KEY_F1;
	if (GetKeyState(DIK_F2) & 0x80)
		m_dwCurKey |= KEY_F2;
	if (GetKeyState(DIK_F3) & 0x80)
		m_dwCurKey |= KEY_F3;
}

VOID CInputMgr::Release()
{
	m_pKeyBoard->Release();
	m_pMouse->Release();
	m_pInputSDK->Release();
}
