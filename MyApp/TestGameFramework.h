#pragma once

class GameFramework;
class CInput;
class CText;
class CTexture;
class CSprite;
class CPayload;
class CPayloadManager;
class CGameObject;

class TestGameFramework :
	public GameFramework
{
public:
	TestGameFramework();
	virtual ~TestGameFramework();

	virtual bool InitFramework(HWND hWnd, HINSTANCE hInstance);
	virtual void ReleaseFramework();

	virtual void LoadTextures();
	virtual void ReleaseTextures();

	virtual void InitGameData();
	virtual void ReleaseGameData();

	virtual void GameUpdate(UINT& escapecode);
	virtual void GameRender();

	void ResetBlock(int index);
	void RemoveInvader(int index);
	bool CanBlockPayloadReload(DWORD dwtTickCount);
	void Update(float dt);
	void ProjectileUpdate(float dt); // pc, ���� �Ѿ� �߻�
	void InvaderCollisionUpdate(float dt); // pc �Ѿ˰� ���� �浹
	void PlayerCollisionUpdate(float dt); // ���� �Ѿ˰� ���� �浹
	void Render();

	void TitleUpdate(float dt);
	void TitleRender();

	void ReadyUpdate(float dt);
	void ReadyRender();

	void GameOverUpdate(float dt);
	void GameOverRender();

	void NextLevelUpdate(float dt);
	void NextLevelRender();

	void DrawRenderObject();
	void DrawScores();

	void isAllDead();

protected:
	LPDIRECT3D9 m_pD3D; // global
	LPDIRECT3DDEVICE9 m_pD3DDevice;
	D3DCOLOR m_ClearColor;
	float m_ScreenWidth;
	float m_ScreenHeight;

	// ��������Ʈ ��ü �������̽� ����
	D3DXVECTOR3 m_PCPos;
	D3DXVECTOR3 m_BlockPos[21];
	D3DXVECTOR3 m_BlockDir;
	D3DXVECTOR3 m_InvaderDir;
	bool m_bInvaderRightDir;

	float PCNormalSpeed;
	float PCPowerUpSpeed;

	float m_BlockSpeed;
	float m_PCSpeed;

	CInput* m_Input;
	CText* m_Text;
	int m_Score;
	int m_Level;
	int m_TakeCount;
	CTexture* m_Texture;
	CSprite* m_Sprite[21]; // 0 pc, 1~9 block or item
	bool m_Pause;
	DWORD m_dwPrevTime;
	float m_ItemKeepTime;

	CPayloadManager* m_PCPayloadManager;
	CPayloadManager* m_InvaderPayloadManager;

	CGameObject* m_Player;
	CGameObject* m_Invader[20];

	D3DXVECTOR3 m_ScorePos;
	float m_ScoreLife;
	// �κ��̴� �Ѿ� �ӵ�
	float m_InvaderShootTimer;

	int m_GameMode;
	int m_InvaderDead;
};