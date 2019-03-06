#include "stdafx.h"
#include <d3d9.h>
#include <d3dx9.h>
#include "CSprite.h"


CSprite::CSprite(LPDIRECT3DDEVICE9 pD3DDevice,
	LPDIRECT3DTEXTURE9 pTexture,
	const D3DXVECTOR3& center)
{
	m_pD3DDevice = pD3DDevice;
	m_pTexture = pTexture;
	m_Color = D3DCOLOR_XRGB(255, 255, 255);
	m_Center = center;

	InitSprite();

}

CSprite::~CSprite()
{
	Release();
}

void CSprite::InitSprite() 
{
	auto hr = D3DXCreateSprite(m_pD3DDevice, &m_pSpr);
	if (FAILED(hr))
		throw 3L; // 3번 에러를 던짐
}
void CSprite::Release()
{
	if (m_pSpr != NULL)
	{
		m_pSpr->Release();
		m_pSpr = NULL;
	}
}

void CSprite::Draw()
{
	m_pSpr->Begin(D3DXSPRITE_ALPHABLEND);
	m_pSpr->Draw(m_pTexture, NULL, &m_Center, &m_Pos, m_Color);
	m_pSpr->End();
}