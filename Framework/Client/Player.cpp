#include "framework.h"
#include "Player.h"
#include "StaticMesh.h"
#include "Bullet.h"
#include "Collision.h"

CPlayer::CPlayer()
{
}

CPlayer::~CPlayer()
{
	Release();
}

HRESULT CPlayer::Initialize()
{
	if (FAILED(CObj::Initialize()))
		return E_FAIL;

	m_pRootSignature = m_pShaderMgr->Get_RootSignature();
	m_pPipelineState = m_pShaderMgr->Get_PipelineState(L"Default");

	m_pTransform = CTransform::Create();
	m_pTransform->Set_Scale({ 0.3f, 0.3f, 0.3f });
	m_pTransform->SetState(CTransform::POSITION, XMVectorSet(-572.027f, 0.f, -72.2364f, 1.f));
	m_pTransform->Rotate_LookAt(m_pTransform->GetState(CTransform::POSITION) + XMVectorSet(0.00226141f, 0.f, -0.299992f, 0.f));

	if (FAILED(Add_Component(CStaticMesh::Create("../Binary/Resources/Yoshi/Yoshi.obj"))))
		return E_FAIL;

	m_pCollision = CCollision::Create(m_pTransform, 10.f, XMVectorSet(0.f, 7.f, 0.f, 0.f));
	if (FAILED(Add_Component(m_pCollision)))
		return E_FAIL;
	m_pGameMgr->Add_Collision(CGameMgr::PLAYER, m_pCollision);

	return NOERROR;
}

INT CPlayer::Update(const FLOAT& fTimeDelta)
{
	KeyInput(fTimeDelta);

	if (m_bKnockBack)
	{
		m_pTransform->Move_Forward(-fTimeDelta * 10.f);
		XMVECTOR vDist = m_pTransform->GetState(CTransform::POSITION) - m_vBeginPos;
		if (abs(XMVectorGetX(vDist)) > 0.7f)
			m_bKnockBack = FALSE;
	}

	if (m_pInputMgr->KeyDown(KEY_F1))
		g_bColliderRender = !g_bColliderRender;

	CObj::Update(fTimeDelta);

	return 0;
}

INT CPlayer::LateUpdate(const FLOAT& fTimeDelta)
{
	for (auto p : m_pGameMgr->Get_CollisionList(CGameMgr::MONSTER))
	{
		if (m_pCollision->Collision_Sphere(p))
		{
			m_bKnockBack = TRUE;
			m_vBeginPos = m_pTransform->GetState(CTransform::POSITION);
		}
	}

	CObj::LateUpdate(fTimeDelta);

	m_pGameMgr->Add_RenderList(this);

	return 0;
}

VOID CPlayer::Render()
{
	auto commandList = m_pDeviceMgr->Get_CommandLst();
	if (!commandList) return;

	commandList->SetGraphicsRootSignature(m_pShaderMgr->Get_RootSignature());
	commandList->SetPipelineState(m_pPipelineState);

	XMMATRIX matView = m_pGameMgr->Get_View();
	XMMATRIX matProj = m_pGameMgr->Get_Proj();
	XMMATRIX matWorld = m_pTransform->GetWorld();

	XMStoreFloat4x4(&m_tConstInfo.World, XMMatrixTranspose(matWorld));
	XMStoreFloat4x4(&m_tObjInfo.View, XMMatrixTranspose(matView));
	XMStoreFloat4x4(&m_tObjInfo.Proj, XMMatrixTranspose(matProj));

	m_pShaderMgr->Get_ObjCBV()->CopyData(0, m_tObjInfo);

	auto objCB = m_pShaderMgr->Get_ObjCBV()->Resource();
	commandList->SetGraphicsRootConstantBufferView(RootParam::OBJ, objCB->GetGPUVirtualAddress());

	commandList->SetGraphicsRoot32BitConstants(RootParam::CONSTANT, 20, &m_tConstInfo, 0);

	m_pLightMgr->Set_LightCBV(commandList);

	for (auto p : m_lstComponent)
		p->Render();
}

VOID CPlayer::KeyInput(const FLOAT& fTimeDelta)
{
	if (m_bKnockBack)
		return;

	if (m_pInputMgr->KeyPressing(KEY_W))
		m_pTransform->Move_Forward(fTimeDelta * 5.f);
	if (m_pInputMgr->KeyPressing(KEY_S))
		m_pTransform->Move_Forward(-fTimeDelta * 5.f);
	if (m_pInputMgr->KeyPressing(KEY_D))
		m_pTransform->Rotate_Axis(m_pTransform->GetState(CTransform::UP), 2.f * fTimeDelta);
	if (m_pInputMgr->KeyPressing(KEY_A))
		m_pTransform->Rotate_Axis(m_pTransform->GetState(CTransform::UP), 2.f * -fTimeDelta);

	if (m_pInputMgr->KeyDown(KEY_SPACE))
	{
		m_pDeviceMgr->Reset_CommandLst();
		m_pGameMgr->Add_GameObj(CGameMgr::BULLET, CBullet::Create(m_pTransform));
		m_pDeviceMgr->Close_CommandLst();
		m_pDeviceMgr->Wait_GPU();
	}

	//if (m_pInputMgr->KeyDown(KEY_ENTER))
	//{
	//	XMVECTOR vPos = m_pTransform->GetState(CTransform::POSITION);
	//	XMVECTOR vLook = m_pTransform->GetState(CTransform::LOOK);
	//	cout << XMVectorGetX(vPos) << ", " << XMVectorGetY(vPos) << ", " << XMVectorGetZ(vPos) << "//";
	//	cout << XMVectorGetX(vLook) << ", " << XMVectorGetY(vLook) << ", " << XMVectorGetZ(vLook) << endl;
	//}
}

VOID CPlayer::Release()
{
	CObj::Release();
}

CPlayer* CPlayer::Create()
{
	CPlayer* pInstance = new CPlayer;
	if (FAILED(pInstance->Initialize()))
		SafeDelete(pInstance);
	return pInstance;
}
