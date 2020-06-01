// stdafx.cpp : source file that includes just the standard includes
//	Color.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"


// 资源加载图像
BOOL LoadImageFromResource(CImage* pImage, UINT nResID, LPCWSTR lpTyp)
{
	if (pImage == NULL)
		return FALSE;

	pImage->Destroy();

	// 查找资源
	HRSRC hRsrc = ::FindResource(GetModuleHandle(NULL), MAKEINTRESOURCE(nResID), lpTyp);
	if (hRsrc == NULL)
		return FALSE;

	// 加载资源
	HGLOBAL hImgData = ::LoadResource(GetModuleHandle(NULL), hRsrc);
	if (hImgData == NULL)
	{
		::FreeResource(hImgData);
		return FALSE;
	}

	// 锁定内存中的指定资源
	LPVOID lpVoid = ::LockResource(hImgData);

	LPSTREAM pStream = NULL;
	DWORD dwSize = ::SizeofResource(GetModuleHandle(NULL), hRsrc);
	HGLOBAL hNew = ::GlobalAlloc(GHND, dwSize);
	LPBYTE lpByte = (LPBYTE)::GlobalLock(hNew);
	::memcpy(lpByte, lpVoid, dwSize);

	// 解除内存中的指定资源
	::GlobalUnlock(hNew);

	// 从指定内存创建流对象
	HRESULT ht = ::CreateStreamOnHGlobal(hNew, TRUE, &pStream);
	if (ht != S_OK)
	{
		GlobalFree(hNew);
	}
	else
	{
		// 加载图片
		pImage->Load(pStream);
		GlobalFree(hNew);
	}

	// 释放资源
	::FreeResource(hImgData);
	return TRUE;
}