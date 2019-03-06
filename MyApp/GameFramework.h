#pragma once
class GameFramework
{
public:
	GameFramework();
	virtual ~GameFramework();

	virtual bool InitFramework(HWND hWnd, HINSTANCE hInstance) = 0;
	virtual void ReleaseFramework() = 0;

	virtual void LoadTextures() = 0;
	virtual void ReleaseTextures() = 0;

	virtual void InitGameData() = 0;
	virtual void ReleaseGameData() = 0;

	virtual void GameUpdate(UINT& escapecode) = 0;
	virtual void GameRender() = 0;

};

