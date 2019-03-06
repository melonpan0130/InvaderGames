#include "stdafx.h"

#include <time.h>
#include <tchar.h>
#include <d3d9.h>
#include <d3dx9.h>
#include<dinput.h>

#include "CInput.h"
#include "CText.h"
#include "CTexture.h"
#include "CSprite.h"
#include "CPayload.h"
#include "CPayloadManager.h"
#include "CGameObject.h"

#include "GameFramework.h"
#include "TestGameFramework.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "winmm.lib") // window media 
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

float g_SpawnXPos[5] = { 60.0f, 180.0f,  300.0f,420.0f, 540.0f };
float g_SpawnYPos[5] = { 50.0f,100.0f, 150.0f, 200.0f, 250.0f };

TestGameFramework::TestGameFramework()
{
	m_pD3D = NULL;
	m_pD3DDevice = NULL;
	D3DCOLOR g_ClearColor = D3DCOLOR_XRGB(0, 0, 225);

	// ��������Ʈ ��ü �������̽� ����
	m_PCPos = D3DXVECTOR3(320, 400, 0);
	//m_BlockDir = D3DXVECTOR3(1, 0, 0);
	

	m_Texture;
	m_Sprite[21]; // 0 pc, 1~9 block or item
	m_Pause = false;
	m_dwPrevTime = 0;
	m_ItemKeepTime = 0;
	m_InvaderShootTimer = 0.f;
	m_GameMode = 1;
	m_InvaderDead = 0;
}

/////////////////////// GameFramework
TestGameFramework::~TestGameFramework()
{
}

// ���� �ʱ�ȭ
bool TestGameFramework::InitFramework(HWND hWnd, HINSTANCE hInstance)
{
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (m_pD3D == NULL)
		return false;

	RECT rc;
	GetClientRect(hWnd, &rc);

	int ClientWidth = rc.right - rc.left + 1;
	int ClientHeight = rc.bottom - rc.top + 1;
	m_ScreenWidth = (float)ClientWidth;
	m_ScreenHeight = (float)ClientHeight;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed = TRUE;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = ClientWidth;
	d3dpp.BackBufferHeight = ClientHeight;

	// dx Device ����
	if (m_pD3D->CreateDevice(D3DADAPTER_DEFAULT
		, D3DDEVTYPE_HAL
		, hWnd
		, D3DCREATE_HARDWARE_VERTEXPROCESSING
		, &d3dpp
		, &m_pD3DDevice) == E_FAIL) 
		return false;

	srand((unsigned int)time(NULL));

	m_Input = new CInput(hWnd, hInstance);
	m_Text = new CText(m_pD3DDevice, 20, 40);
	m_Texture = new CTexture(m_pD3DDevice);
	m_InvaderDir = D3DXVECTOR3(1, 0, 0);

	return true;
}

void TestGameFramework::ReleaseFramework()
{
	delete m_Input;
	delete m_Text;
	delete m_Texture;

	if (m_pD3DDevice != NULL)
		m_pD3DDevice->Release();

	if (m_pD3D != NULL)
		m_pD3D->Release();

	// �����Ͱ��̱� ������ null�� �ʱ�ȭ
	m_pD3DDevice = NULL;
	m_pD3D = NULL;
}

void TestGameFramework::LoadTextures()
{
	m_Texture->LoadTexture(0, TEXT("..\\image\\myship.bmp"));
	m_Texture->LoadTexture(1, TEXT("..\\image\\enemy.bmp"));
	m_Texture->LoadTexture(2, TEXT("..\\image\\myshoot.bmp")); // �Ѿ�
	m_Texture->LoadTexture(3, TEXT("..\\image\\enemyshoot.bmp")); // �Ѿ�
}
void TestGameFramework::ReleaseTextures()
{}

void TestGameFramework::InitGameData()
{
	D3DXVECTOR3 pcPos = D3DXVECTOR3(m_ScreenWidth*0.5f, m_ScreenHeight - 64.f, 0);
	D3DXVECTOR3 center = D3DXVECTOR3(32, 32, 0);
	D3DXVECTOR3 payloadcenter = D3DXVECTOR3(20, 20, 0);
	D3DXVECTOR3 pcshootdir = D3DXVECTOR3(0, -1, 0);
	D3DXVECTOR3 invadershootdir = D3DXVECTOR3(0, 1, 0);
	const float pcspeed = 200.f;
	const float invaderspeed = 100.f;
	const float pcshootspeed = 100.f;
	const float invadershootspeed = 50.f;

	PCNormalSpeed = 200.0f;
	PCPowerUpSpeed = 400.0f;

	m_BlockSpeed = 60.0f;
	m_PCSpeed = PCNormalSpeed;

	m_Input;
	m_Text;
	m_Score = 0;
	m_Level = 0;
	m_TakeCount = 0;

	m_dwPrevTime = GetTickCount();
	m_bInvaderRightDir = true;
	m_ScoreLife = 0.f;
	
	// Player
	m_Player = new CGameObject(m_pD3DDevice
		, m_Texture->GetTexture(0)
		, center
		, pcPos, pcspeed);

	// ó�� ������ ��ġ(���)
	for (int i = 0; i < 20; ++i)// Block
	{
		m_Invader[i] = new CGameObject(m_pD3DDevice
			, m_Texture->GetTexture(1), center
			, D3DXVECTOR3(40.f + 32.f + (float)((i % 5) * 100)
			, 20.f + 32.f + (float)((i / 5) * 64), 0.f)
			, invaderspeed);
		m_Invader[i]->SetAlive(true);
	}

	// shoot sprite
	m_PCPayloadManager = new CPayloadManager(m_pD3DDevice,
		m_Texture->GetTexture(2), payloadcenter, pcshootspeed, pcshootdir, D3DXVECTOR2(m_ScreenWidth, m_ScreenHeight));
	m_InvaderPayloadManager = new CPayloadManager(m_pD3DDevice,
		m_Texture->GetTexture(3), payloadcenter, invadershootspeed, invadershootdir, D3DXVECTOR2(m_ScreenWidth, m_ScreenHeight));
	m_InvaderShootTimer = 1.f;

}

void TestGameFramework::ReleaseGameData()
{
	delete m_PCPayloadManager;
	delete m_InvaderPayloadManager;

	m_PCPayloadManager = NULL;
	m_InvaderPayloadManager = NULL;

	delete m_Player;
	m_Player = NULL;

	for (int i = 0; i < 20; ++i) {
		delete m_Invader[i];
		m_Invader[i] = NULL;
	}
}

void TestGameFramework::GameUpdate(UINT& escapecode)
{
	if (m_Input->IsEscapePressed())
	{
		escapecode = WM_QUIT;
		return;
	}
	else 
	{
		m_Input->ReadKeyboard();
		if (m_Input->IsEscapePressed())
			m_Pause = !m_Pause;
		else
		{
			m_Input->ReadKeyboard();

			if (m_Input->IsPausePressed())
				m_Pause = !m_Pause;

			DWORD dwCurTime = GetTickCount();
			DWORD dwDt = dwCurTime - m_dwPrevTime;
			float fDt = (float)(dwDt)*0.001f;
			m_dwPrevTime = dwCurTime;
			if (m_Pause == false) // g_Pause�� false�϶��� update
			{
				switch (m_GameMode) {
				case 1:
					TitleUpdate(fDt);
					break;
				case 2:
					ReadyUpdate(fDt);
					break;
				case 3:
					if (m_Pause == false && m_Input->IsSpacePressed())
					{
						m_PCPayloadManager->OnFire(m_Player->GetPosition());
					}

					Update(fDt);
					ProjectileUpdate(fDt); // pc, ���� �Ѿ� �߻�
					InvaderCollisionUpdate(fDt); // pc �Ѿ˰� ���� �浹
					PlayerCollisionUpdate(fDt); // ���� �Ѿ˰� ���� �浹
					break;
				case 4:
					GameOverUpdate(fDt);
					break;
				case 5: // next level
					NextLevelUpdate(fDt);
					break;
				} // switch
			}
		}
	}
}

void TestGameFramework::GameRender()
{
	switch (m_GameMode) {
	case 1:
		// ���� Ÿ��Ʋ
		TitleRender();
		break;
	case 2:
		// ���� ����
		ReadyRender();
		break;
	case 3:
		// ����
		Render();
		break;
	case 4: 
		// ���� ����
		GameOverRender();
		break;
	case 5:
		NextLevelRender();
		break;
	}
} // gamerender

///////////////////////////// Game ///////////////////////////////
void TestGameFramework::ResetBlock(int index) 
{ // ��� ����
	//m_BlockPos[index] = D3DXVECTOR3(50.0f,50.0f, 0);
	// m_Invader[index].
		//->SetTexture(m_Texture->GetTexture(3));
	// m_Invader[index].

}

void TestGameFramework::RemoveInvader(int index)
{
	
	//m_Invader[index]->SetPosition
}

bool TestGameFramework::CanBlockPayloadReload(DWORD dwtTickCount)
{
	static DWORD dwPrevTime = 0;
	DWORD dt = GetTickCount() - dwPrevTime;
	if (dt > dwtTickCount)
	{
		dwPrevTime = GetTickCount();
		return true;
	}
	return false;
}

void TestGameFramework::TitleUpdate(float dt)
{
	if (m_Input->IsSpacePressed())
	{
		m_Input->ClearKeyBuffer();
		m_GameMode = 2;
	}
}

void TestGameFramework::TitleRender()
{
	if (m_pD3DDevice == NULL)
		return;

	// ����� �����
	m_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET
		, m_ClearColor, 1.0f, 0);

	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		m_Text->Draw(0, 0, (int)m_ScreenWidth
			, (int)(m_ScreenHeight*0.5f), _T("Space Invaders"), D3DCOLOR_XRGB(255, 255, 0));

		m_Text->Draw(0, (int)(m_ScreenHeight*0.5f)
			, (int)m_ScreenWidth, (int)(m_ScreenHeight*0.5f)
			, _T("PRESS SPACE KEY TO START"), D3DCOLOR_XRGB(255, 255, 255));

		m_pD3DDevice->EndScene();
	}

	// ����� ����
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
	InitGameData();

}

void TestGameFramework::ReadyUpdate(float dt)
{
	static float fAccTime = 0.f;
	fAccTime += dt;
	if (fAccTime > 1.f)
	{
		m_GameMode = 3;
		fAccTime = 0.1f;
	}
}

void TestGameFramework::ReadyRender()
{
	if (m_pD3DDevice == NULL)
		return;

	//����� �����
	m_pD3DDevice->Clear(0, NULL
		, D3DCLEAR_TARGET
		, m_ClearColor
		, 1.0f, 0);

	DrawRenderObject();

	//TCHAR szScore[50];
	//_stprintf_s(szScore, 50, _T("Lv : %d, Score : %d"), m_Level + 1, m_Score);
	m_Text->Draw(0, 0, (int)m_ScreenWidth, (int)m_ScreenHeight, _T("Ready"), D3DCOLOR_XRGB(255, 255, 0));

	// ����� ����
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL); // surface ���
}

void TestGameFramework::GameOverUpdate(float dt)
{
	if (m_Input->IsSpacePressed())
	{
		m_Input->ClearKeyBuffer();
		m_GameMode = 1;

		ReleaseGameData();
	}
}
void TestGameFramework::GameOverRender()
{
	if (m_pD3DDevice == NULL)
		return;

	//����� �����
	m_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET
		, m_ClearColor, 1.0f, 0);

	DrawRenderObject();

	m_Text->Draw(0, 0, (int)m_ScreenWidth, (int)m_ScreenHeight, _T("GameOver"), D3DCOLOR_XRGB(255, 255, 0));

	// ����� ����
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL); // surface ���
}

void TestGameFramework::NextLevelUpdate(float dt)
{
	if (m_Input->IsSpacePressed())
	{
		m_Input->ClearKeyBuffer();
		m_GameMode = 2;
		m_Level++;
		m_InvaderDead = 0;

		ReleaseGameData();
		InitGameData();
	}
}

void TestGameFramework::NextLevelRender()
{
	if (m_pD3DDevice == NULL)
		return;

	//����� �����
	m_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET
		, m_ClearColor, 1.0f, 0);

	DrawRenderObject();

	m_Text->Draw(0, 0, (int)m_ScreenWidth, (int)m_ScreenHeight, _T("Next Level"), D3DCOLOR_XRGB(255, 255, 0));

	// ����� ����
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL); // surface ���
}

bool toright = true;
// dt : dellta_time �Լ��� �Լ� ���� ������ ���� �ð� ��
void TestGameFramework::Update(float dt)
{
	D3DXVECTOR3 pcDir(0.f, 0.f, 0.f);
	m_Input->GetArrowDir(pcDir);

	m_Player->SetDirection(pcDir.x, pcDir.y);
	m_Player->Update(dt);

	// ȭ�� �̵� ����
	m_Player->ArrangePosition(32.f, m_ScreenWidth - 32.f);

	// Invader �̵�
	for (int i = 0; i < 20; i++)
	{
		m_Invader[i]->SetDirection(m_InvaderDir.x, m_InvaderDir.y);
		m_Invader[i]->Update(dt);

		// ����ִ� ������ ã��
		for (int i = 0; i < 20; i++)
		{
			if (m_Invader[i]->IsTouched(32.f, m_ScreenWidth - 32.f, m_bInvaderRightDir) == true)
			{
				m_InvaderDir = D3DXVECTOR3((m_bInvaderRightDir) ? -1.f : 1.f, 0.f, 0.f);
				m_bInvaderRightDir = !m_bInvaderRightDir;

				for (int n = 0; n < 20; n++)
				{
					D3DXVECTOR3 pos = m_Invader[n]->GetPosition();
					m_Invader[n]->SetPosition(pos + D3DXVECTOR3(0, 32.f, 0)); // ������
				}
				break;
			}
		}

		if (m_ScoreLife > 0.0f)
		{
			m_ScoreLife -= dt;
			m_ScorePos.y -= 80.f*dt;
		}
		
		// �Ѿ� ������
		ProjectileUpdate(dt);
	}
}

void TestGameFramework::ProjectileUpdate(float dt)// pc, ���� �Ѿ� �߻�
{
	// pc �Ѿ� �߻�
	m_PCPayloadManager->Update(dt);
	
	// �κ����� �Ѿ� �߻�
	// 1�ʿ� �Ѹ����� �߻�
	// �����Ͽ� ���ϱ�
	int shoot;
	while (true) {
		shoot = (int)rand() % 20;
		// ����ִ� ���� �߿��� �߻�
		if (m_Invader[shoot]->GetAlive() == true)
			 break;
	}

	D3DXVECTOR3 dir = m_Player->GetPosition() - m_Invader[shoot]->GetPosition();

	m_InvaderShootTimer += dt;
	if (m_InvaderShootTimer > 10.f)
	{
		m_InvaderShootTimer -= 10.f;

		int shooter = 0; // �� ���
		int aliveCount = 0;// ����� ��
		int aliveIndecis[20]; // ���� �� �� �ִ� ���
		for (int i = 0; i < 20; i++)
		{
			if (m_Invader[i]->GetAlive())
				aliveIndecis[aliveCount++] = i;
		}
		
		if (aliveCount > 0)
		{/*
			// ��� �ѽ��
			shooter = rand() % aliveCount; // �ѽ� ���
			m_InvaderPayloadManager->OnFire(
				m_Invader[aliveIndecis[shooter]]->GetPosition() + D3DXVECTOR3(0.f, 32.f, 0.f)
				, D3DXVECTOR3(0.f, 1.f, 0.f));*/
		}
	}
	m_InvaderPayloadManager->Update(dt);
} // ProjectileUpdate

void TestGameFramework::InvaderCollisionUpdate(float dt) // pc �Ѿ˰� ���� �浹
{
	// �浹ó��
	for (int i = 0; i < 20; i++)
	{
		// �Ѿ˰� ���� �浹 ���
		if ((m_Invader[i]->GetAlive() == true) && m_PCPayloadManager->IsCollision(m_Invader[i]->GetPosition(), (32.f + 8.f)))
		{
			m_Invader[i]->SetAlive(false);
			m_InvaderDead++;
			// ���ھ� ����
			m_ScorePos = m_Invader[i]->GetPosition();
			m_ScoreLife = 0.3f;
		}
	}// for

	// ��� �׾��� �� ���� ����
	if (m_InvaderDead == 20)
		m_GameMode = 5;
}

void TestGameFramework::PlayerCollisionUpdate(float dt) // ���� �Ѿ˰� ���� �浹
{
	if (m_InvaderPayloadManager->IsCollision(m_Player->GetPosition(), (32.f + 0.f)) == true)
	{
		m_Score -= 100;
		m_GameMode = 4;
	}
}

void TestGameFramework::Render()
{
	if (m_pD3DDevice == NULL)
		return;

	//����� �����
	m_pD3DDevice->Clear(0, NULL
		, D3DCLEAR_TARGET
		, m_ClearColor
		, 1.0f, 0);

	DrawRenderObject();
	DrawScores();

	// ����� ����
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL); // surface ���
}

void TestGameFramework::DrawRenderObject()
{
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		// �����
		m_Player->Render();

		// block
		for (int i = 0; i < 20; ++i)
		{
			if (m_Invader[i]->GetAlive())
				m_Invader[i]->Render();
		}

		// �Ѿ�
		m_PCPayloadManager->Draw();
		m_InvaderPayloadManager->Draw();

		m_pD3DDevice->EndScene();
	}
}

void TestGameFramework::DrawScores()
{
	TCHAR szScore[50];
	_stprintf_s(szScore, 50, _T("Lv : %d, Score : %d"), m_Level + 1, m_InvaderDead); // m_Score
	m_Text->DrawLT(10, 10, 500, 80, szScore);

	if (m_ScoreLife > 0.f)
	{
		m_Text->Draw((int)(m_ScorePos.x), (int)(m_ScorePos.y - 10.f), 40, 32, _T("100"));
	}
}