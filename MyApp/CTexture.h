#pragma once
#define MAX_SIZE 10

class CTexture
{
protected:
	LPDIRECT3DDEVICE9 m_pD3DDeivce;
	LPDIRECT3DTEXTURE9 m_pTextures[MAX_SIZE];

public:
	explicit CTexture(LPDIRECT3DDEVICE9 pD3DDeivce);
	virtual ~CTexture();

	int LoadTexture(int slot, const TCHAR* fliename);
	void ReleaseAll();

	LPDIRECT3DTEXTURE9 GetTexture(int slot) const; // 참조하는 변수를 변경불가
};

