#include "framework.h"
#include "Scene_Stage.h"

CScene_Stage::CScene_Stage()
{
}

CScene_Stage::~CScene_Stage()
{
	Release();
}

HRESULT CScene_Stage::Initialize()
{
	if (FAILED(CScene::Initialize())) return E_FAIL;

	//m_pDeviceMgr->Reset_CommandLst();

	//m_pTest = CMesh::Create("../Binary/Resources/YoshiCart/torokko_yoshi_body01a.obj", L"Color");
	//m_pTest = CMesh::Create("../Binary/Resources/Apple/Apple.obj", L"Color");
	//m_pTest = CMesh::Create("../Binary/Resources/cube.obj", L"Color");
	//m_pTest = CMesh::Create("../Binary/Resources/1Ho/1Ho.obj", L"Color");

	//m_pDeviceMgr->Close_CommandLst();
	//m_pDeviceMgr->Wait_GPU();

	cout << "StageScene" << endl;

	return NOERROR;
}

INT CScene_Stage::Update(const FLOAT& fTimeDelta)
{
	return 0;
}

VOID CScene_Stage::Release()
{
	m_pGameMgr->Clear_ObjList();
}

CScene_Stage* CScene_Stage::Create()
{
	CScene_Stage* pInstance = new CScene_Stage;
	if (FAILED(pInstance->Initialize()))
		SafeDelete(pInstance);
	return pInstance;
}
