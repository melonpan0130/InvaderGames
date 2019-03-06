#pragma once
class CSprite
{
protected:
	LPDIRECT3DDEVICE9 m_pD3DDevice;
	LPD3DXSPRITE m_pSpr;
	LPDIRECT3DTEXTURE9 m_pTexture;

	D3DXVECTOR3 m_Pos; // float
	D3DXVECTOR3 m_Center;
	D3DCOLOR m_Color; // byte
	bool m_Isitem = false;

public:
	explicit CSprite(LPDIRECT3DDEVICE9 pD3DDevice,
									LPDIRECT3DTEXTURE9 pTexture,
									const D3DXVECTOR3& center);
	virtual ~CSprite();

	void InitSprite();
	void Release();

	void SetPosition(const D3DXVECTOR3& pos)
	{
		m_Pos = pos;
	}
	void SetTexture(LPDIRECT3DTEXTURE9 pTexture)
	{
		m_pTexture = pTexture;
	}

	void SetItem(bool flag) {
		m_Isitem = flag;
	}
	bool IsItem() {
		return m_Isitem;
	}
	void Draw();
};