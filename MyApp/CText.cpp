#include "stdafx.h"
#include <d3d9.h>
#include<d3dx9.h>
#include "CText.h"
// 내가 만든 해더는 맨 마지막에 넣자

CText::CText(LPDIRECT3DDEVICE9 pD3DDivice, int width, int height)
{
	D3DXFONT_DESC desc;
	ZeroMemory(&desc, sizeof(D3DXFONT_DESC));

	desc.CharSet = HANGUL_CHARSET;
	desc.Height = height;
	desc.Width = width;
	desc.Weight = FW_NORMAL; // bord도 사용 가능
	desc.Quality = DEFAULT_QUALITY;
	desc.MipLevels = 1; // 큰이미지를 줄여서 자글거리는 느낌을 줄임

	auto hr = D3DXCreateFontIndirect(pD3DDivice, &desc, &m_pFont);
	// hr :  result 값을 가져오고 버려지는 변수,,
	if (FAILED(hr))
		throw 2L;
}

CText::~CText()
{
	if (m_pFont != NULL)
	{ // 항상 null이 아닌지 확인하고
		m_pFont->Release(); // 지우기
		m_pFont = NULL;
	}
}

void CText::Draw(int x, int y, int width, int height, const TCHAR* lpszText
, D3DCOLOR color) 
{
	RECT rc;
	SetRect(&rc, x, y, x + width, y + height);

	m_pFont->DrawTextW(NULL, lpszText, -1, &rc, DT_CENTER | DT_VCENTER, color);
}

void CText::DrawLT(int x, int y, int width, int height, const TCHAR* lpszText
	, D3DCOLOR color /*= D3DCOLOR_XRGB(255, 0, 0)*/)
{
	RECT rc;
	SetRect(&rc, x, y, x + width, y + height);
	m_pFont->DrawTextW(NULL, lpszText, -1, &rc, DT_LEFT|DT_TOP, color);
}