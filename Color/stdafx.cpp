// stdafx.cpp : source file that includes just the standard includes
//	Color.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"


// ��Դ����ͼ��
BOOL LoadImageFromResource(CImage* pImage, UINT nResID, LPCWSTR lpTyp)
{
	if (pImage == NULL)
		return FALSE;

	pImage->Destroy();

	// ������Դ
	HRSRC hRsrc = ::FindResource(GetModuleHandle(NULL), MAKEINTRESOURCE(nResID), lpTyp);
	if (hRsrc == NULL)
		return FALSE;

	// ������Դ
	HGLOBAL hImgData = ::LoadResource(GetModuleHandle(NULL), hRsrc);
	if (hImgData == NULL)
	{
		::FreeResource(hImgData);
		return FALSE;
	}

	// �����ڴ��е�ָ����Դ
	LPVOID lpVoid = ::LockResource(hImgData);

	LPSTREAM pStream = NULL;
	DWORD dwSize = ::SizeofResource(GetModuleHandle(NULL), hRsrc);
	HGLOBAL hNew = ::GlobalAlloc(GHND, dwSize);
	LPBYTE lpByte = (LPBYTE)::GlobalLock(hNew);
	::memcpy(lpByte, lpVoid, dwSize);

	// ����ڴ��е�ָ����Դ
	::GlobalUnlock(hNew);

	// ��ָ���ڴ洴��������
	HRESULT ht = ::CreateStreamOnHGlobal(hNew, TRUE, &pStream);
	if (ht != S_OK)
	{
		GlobalFree(hNew);
	}
	else
	{
		// ����ͼƬ
		pImage->Load(pStream);
		GlobalFree(hNew);
	}

	// �ͷ���Դ
	::FreeResource(hImgData);
	return TRUE;
}