#include "stdafx.h"
#include <d3d9.h>
#include <d3dx9.h>
#include "CTexture.h"


CTexture::CTexture(LPDIRECT3DDEVICE9 pD3DDeivce)
{
	m_pD3DDeivce = pD3DDeivce;

	for (int i = 0; i < MAX_SIZE; i++) {
		m_pTextures[i] = NULL;
	}
}


CTexture::~CTexture()
{
}

int CTexture::LoadTexture(int slot, const TCHAR* filename) 
{
	if (slot == -1)
	{
		for (int i = 0; i < MAX_SIZE; i++)
			if (m_pTextures[i] == NULL)
			{
				slot = i;
				break;
			}
	}

	auto hr = D3DXCreateTextureFromFileEx(m_pD3DDeivce, filename, 0, 0, 0, 0,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		D3DX_FILTER_NONE,
		D3DX_DEFAULT,
		D3DCOLOR_XRGB(0, 0, 0), // 이 색상값을 무시함
		NULL, NULL, &m_pTextures[slot]);
	if (FAILED(hr))
		return -1;
	return slot;
}
void CTexture::ReleaseAll()
{
	for (int i = 0; i < MAX_SIZE; i++) 
	{
		if (m_pTextures[i] != NULL)
		{
			m_pTextures[i]->Release();
			m_pTextures[i] = NULL;
		}
	}
}

LPDIRECT3DTEXTURE9 CTexture::GetTexture(int slot) const
{
	if (slot < MAX_SIZE)
		return m_pTextures[slot];
	return NULL;
}
