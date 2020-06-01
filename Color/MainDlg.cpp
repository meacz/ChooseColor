// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "MainDlg.h"

CMainDlg::CMainDlg()
{
	m_bColor   = FALSE;
	m_bHue     = FALSE;
	m_bAlpha   = FALSE;
	m_bText    = FALSE;
	row_buffer = new DWORD[256];
}

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
	UIUpdateChildWindows();
	return FALSE;
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CRect rect;
	GetClientRect(&rect);
	ClientToScreen(rect);
	CRgn r = CreateRectRgnIndirect(rect);
	SetWindowRgn(r);
	SetWindowText(TEXT("Color Picker"));
	

	m_ok.SubclassWindow(GetDlgItem(IDOK));
	m_close.SubclassWindow(GetDlgItem(IDC_CLOSE));
	m_min.SubclassWindow(GetDlgItem(IDC_MIN));
	m_grColor.SubclassWindow(GetDlgItem(IDC_GRCOLOR));
	m_grHsl.SubclassWindow(GetDlgItem(IDC_GRHSL));
	m_grRgb.SubclassWindow(GetDlgItem(IDC_GRRGB));
	m_grHsv.SubclassWindow(GetDlgItem(IDC_GRHSV));
	m_cancel.SubclassWindow(GetDlgItem(IDCANCEL));

	m_eRed.SubclassWindow(GetDlgItem(IDC_RED));
	m_eBlue.SubclassWindow(GetDlgItem(IDC_BLUE));
	m_eGreen.SubclassWindow(GetDlgItem(IDC_GREEN));

	m_eAss.SubclassWindow(GetDlgItem(IDC_ASS));
	m_eHtml.SubclassWindow(GetDlgItem(IDC_HTML));
	m_eAlpha.SubclassWindow(GetDlgItem(IDC_ALPHA));

	m_eHue.SubclassWindow(GetDlgItem(IDC_HEEL));
	m_eSaturation.SubclassWindow(GetDlgItem(IDC_SSTA));
	m_eLightness.SubclassWindow(GetDlgItem(IDC_SLIGHT));

	m_eHue2.SubclassWindow(GetDlgItem(IDC_HUE));
	m_eSaturation2.SubclassWindow(GetDlgItem(IDC_SATURATION));
	m_eValue.SubclassWindow(GetDlgItem(IDC_VALUE));

	m_combomenu.SubclassWindow(GetDlgItem(IDC_COMMENU));
	m_combo.SubclassWindow(GetDlgItem(IDC_COMBOCOLOR));
	m_combo.AddString(TEXT("RGB/红"));
	m_combo.AddString(TEXT("RGB/绿"));
	m_combo.AddString(TEXT("RGB/蓝"));
	m_combo.AddString(TEXT("HSV/色调"));
	m_combo.AddString(TEXT("HSV/饱和度"));
	m_combo.AddString(TEXT("HSV/明度"));

	m_combomenu.AddString(TEXT("关于"));
	m_combo.SetCurSel(3);

	m_drop.Create(m_hWnd);      // Alpha wnd
	m_selcol.Create(m_hWnd);    // 屏幕取色
	m_piccol.Create(m_hWnd);    // 常用颜色

	m_drop.m_selCol = m_selcol;
	m_selcol.m_par = m_hWnd;
	m_piccol.m_par = m_hWnd;

	m_cDrop = LoadCursor(GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_CURDROP));
	m_DropPng.Load(IDB_DROPPER, TEXT("PNG"), GetModuleHandle(NULL));

	// Color
	CurrCol = {
		255, 255, 255,  // r g b
		0, 0, 100,		// h s v
		0, 0, 100,		// h s l
		255 };			// alpha
	UpdateValues(GetCurrentColour());

	// cursor
	m_arrow = LoadCursor(GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_BLUE_ARROW));

	CenterWindow();
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	UIAddChildWindowContainer(m_hWnd);
	return TRUE;
}

HBRUSH CMainDlg::OnCtlColorEdit(CDCHandle dc, CEdit edit)
{
	dc.SetBkColor(COLOR_DLG_BK2);
	dc.SetTextColor(COLOR_EDIT_TEXT);
	return (HBRUSH)GetStockObject(NULL_BRUSH);
}

LRESULT CMainDlg::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CPaintDC dc(m_hWnd);
	CRect rtBk, rt;
	GetClientRect(rtBk);
	rt = rtBk;
	// 对话框背景
	dc.FillSolidRect(rtBk, COLOR_DLG_BK1);
	rt.DeflateRect(4, m_ncHeight, 4, 4);
	dc.FillSolidRect(rt, COLOR_DLG_BK2);


	HWND hWndColor, hWndColor2, hWndDropper, hWndAlpha, hWndsel;
	CDC hdc, hcomp;
	CBitmap hbmp;
	CRect rect, rtSilder, rtPer, rtCen, rtBor;
	CPen pen;
	CBrush brush;
	int red, green, blue, alpha;

// Color1  *****

	hWndColor = GetDlgItem(IDC_PCOLOR);
	hdc = ::GetDC(hWndColor);
	hcomp = CreateCompatibleDC(hdc);

	::GetWindowRect(hWndColor, &rect);

	rtBor = rect;
	ScreenToClient(rtBor);
	pen = CreatePen(PS_SOLID, 1, COLOR_SPACE_BORDER);
	dc.SelectPen(pen);
	dc.SelectStockBrush(NULL_BRUSH);
	rtBor.InflateRect(1, 1, 0, -1);
	dc.Rectangle(rtBor);

	red = GetDlgItemInt(IDC_RED);
	green = GetDlgItemInt(IDC_GREEN);
	blue = GetDlgItemInt(IDC_BLUE);
	alpha = GetDlgItemInt(IDC_ALPHA);

// RGB ALL  *****

	if (m_combo.GetCurSel() == 0 || 
		m_combo.GetCurSel() == 1 ||
		m_combo.GetCurSel() == 2)
	{
		DoDrawRGB();
		hcomp.SelectBitmap(bmp_handle);

		// RGB rect
		if(m_combo.GetCurSel() == 0)
			rtCen.SetRect(green - 1, blue - 2, green + 2, blue + 1);  // red
		if (m_combo.GetCurSel() == 1)
			rtCen.SetRect(red - 1, blue - 2, red + 2, blue + 1);      // green
		if (m_combo.GetCurSel() == 2)
			rtCen.SetRect(green - 1, red - 2, green + 2, red + 1);    // blue

		pen = CreatePen(PS_SOLID, 1, RGB(255 - red, 255 - green, 255 - blue));
		brush = CreateSolidBrush(RGB(255 - red, 255 - green, 255 - blue));
		hcomp.SelectPen(pen);
		hcomp.SelectBrush(brush);
		hcomp.SetROP2(R2_NOT);

		hcomp.Rectangle(CRect(rtCen.left - 4, rtCen.top, rtCen.left, rtCen.bottom));		// left
		hcomp.Rectangle(CRect(rtCen.left, rtCen.top - 4, rtCen.right, rtCen.top));			// top
		hcomp.Rectangle(CRect(rtCen.right, rtCen.top, rtCen.right + 4, rtCen.bottom));		// right
		hcomp.Rectangle(CRect(rtCen.left, rtCen.bottom, rtCen.right, rtCen.bottom + 4));	// bottom

	}


// HSV H  *****

	if (m_combo.GetCurSel() == 3)
	{
		DoDrawHSV_Hue();
		hcomp.SelectBitmap(bmp_handle);


		// Draws rect
		double step;
		SColour tempcol;

		step = 100.0 / 255.0;

		tempcol.s = (unsigned short)GetDlgItemInt(IDC_SATURATION);
		tempcol.v = (unsigned short)GetDlgItemInt(IDC_VALUE);
		tempcol.UpdateRGB();

		rtCen.SetRect(
			(int)(tempcol.s / step) - 1, 255 - (int)(tempcol.v / step) - 1,
			(int)(tempcol.s / step) + 2, 255 - (int)(tempcol.v / step) + 2);
		pen = CreatePen(PS_SOLID, 1, RGB(255 - tempcol.r, 255 - tempcol.g, 255 - tempcol.b));
		brush = CreateSolidBrush(RGB(255 - tempcol.r, 255 - tempcol.g, 255 - tempcol.b));
		hcomp.SelectPen(pen);
		hcomp.SelectBrush(brush);
		hcomp.SetROP2(R2_NOT);

		hcomp.Rectangle(CRect(rtCen.left - 4, rtCen.top, rtCen.left, rtCen.bottom));		// left
		hcomp.Rectangle(CRect(rtCen.left, rtCen.top - 4, rtCen.right, rtCen.top));			// top
		hcomp.Rectangle(CRect(rtCen.right, rtCen.top, rtCen.right + 4, rtCen.bottom));		// right
		hcomp.Rectangle(CRect(rtCen.left, rtCen.bottom, rtCen.right, rtCen.bottom + 4));	// bottom

	}

// HSV S  *****

	if (m_combo.GetCurSel() == 4)
	{
		DoDrawHSV_Sat();
		hcomp.SelectBitmap(bmp_handle);


		// Draws rect
		double step, step2;
		SColour tempcol;

		step = 100.0 / 255.0;
		step2 = 359.0 / 255.0;

		// Draws rect
		tempcol.h = (unsigned short)GetDlgItemInt(IDC_HUE);
		tempcol.v = (unsigned short)GetDlgItemInt(IDC_VALUE);
		tempcol.UpdateRGB();
		
		rtCen.SetRect(
			(int)(tempcol.h / step2) - 1, 255 - (int)(tempcol.v / step) - 2,
			(int)(tempcol.h / step2) + 2, 255 - (int)(tempcol.v / step) + 1);
		pen = CreatePen(PS_SOLID, 1, RGB(255 - tempcol.r, 255 - tempcol.g, 255 - tempcol.b));
		brush = CreateSolidBrush(RGB(255 - tempcol.r, 255 - tempcol.g, 255 - tempcol.b));
		hcomp.SelectPen(pen);
		hcomp.SelectBrush(brush);
		hcomp.SetROP2(R2_NOT);
		
		hcomp.Rectangle(CRect(rtCen.left - 4, rtCen.top, rtCen.left, rtCen.bottom));		// left
		hcomp.Rectangle(CRect(rtCen.left, rtCen.top - 4, rtCen.right, rtCen.top));			// top
		hcomp.Rectangle(CRect(rtCen.right, rtCen.top, rtCen.right + 4, rtCen.bottom));		// right
		hcomp.Rectangle(CRect(rtCen.left, rtCen.bottom, rtCen.right, rtCen.bottom + 4));	// bottom
		
	}

// HSV V  *****

	if (m_combo.GetCurSel() == 5)
	{
		DoDrawHSV_Val();
		hcomp.SelectBitmap(bmp_handle);


		// Draws rect
		double step, step2;
		SColour tempcol;

		step = 100.0 / 255.0;
		step2 = 359.0 / 255.0;

		// Draws rect
		tempcol.h = (unsigned short)GetDlgItemInt(IDC_HUE);
		tempcol.s = (unsigned short)GetDlgItemInt(IDC_SATURATION);
		tempcol.UpdateRGB();

		rtCen.SetRect(
			(int)(tempcol.h / step2) - 1, 255 - (int)(tempcol.s / step) - 2,
			(int)(tempcol.h / step2) + 2, 255 - (int)(tempcol.s / step) + 1);
		pen = CreatePen(PS_SOLID, 1, RGB(255 - tempcol.r, 255 - tempcol.g, 255 - tempcol.b));
		brush = CreateSolidBrush(RGB(255 - tempcol.r, 255 - tempcol.g, 255 - tempcol.b));
		hcomp.SelectPen(pen);
		hcomp.SelectBrush(brush);
		hcomp.SetROP2(R2_NOT);

		hcomp.Rectangle(CRect(rtCen.left - 4, rtCen.top, rtCen.left, rtCen.bottom));		// left
		hcomp.Rectangle(CRect(rtCen.left, rtCen.top - 4, rtCen.right, rtCen.top));			// top
		hcomp.Rectangle(CRect(rtCen.right, rtCen.top, rtCen.right + 4, rtCen.bottom));		// right
		hcomp.Rectangle(CRect(rtCen.left, rtCen.bottom, rtCen.right, rtCen.bottom + 4));	// bottom

	}

	hdc.BitBlt(0, 0, rect.right, rect.bottom, hcomp, 0, 0, SRCCOPY);


// Color2  *****

	hWndColor2 = GetDlgItem(IDC_PSILDER);
	hdc = ::GetDC(hWndColor2);
	hcomp = CreateCompatibleDC(hdc);

	::GetClientRect(hWndColor2, &rect);

	hbmp = CreateCompatibleBitmap(hdc, rect.right, 255);
	hcomp.SelectBitmap(hbmp);

	brush = CreateSolidBrush(COLOR_DLG_BK2);
	hcomp.FillRect(rect, brush);


// RGB R  *****

	if (m_combo.GetCurSel() == 0)
	{
		// draw silder
		OnDrawSilder();
		CDC m_dc = CreateCompatibleDC(hcomp);
		m_dc.SelectBitmap(bmp_handle2);
		hcomp.BitBlt(6, 0, rect.right, rect.bottom, m_dc, 0, 0, SRCCOPY);


		// RED arrows
		brush = CreateSolidBrush(COLOR_SPACE_BORDER);
		pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));

		POINT pt[] = {
			rect.right - 1, 255 - ((int)(red)-4),
			rect.right - 1, 255 - ((int)(red)+4),
			rect.right - 5, 255 - ((int)(red)),
			rect.right - 1, 255 - ((int)(red)-4)
		};
		CRgn rgn;
		rgn.CreatePolygonRgn(pt, 4, WINDING);
		hcomp.FillRgn(rgn, brush);
		rtSilder.SetRect(rect.left + 5, 255 - ((int)(red)+1), rect.right - 5, 255 - ((int)red - 2));
		hcomp.SelectPen(pen);
		hcomp.SelectBrush(brush);
		hcomp.Rectangle(rtSilder);
	}

// RGB G  *****

	if (m_combo.GetCurSel() == 1)
	{
		// draw silder
		OnDrawSilder();
		CDC m_dc = CreateCompatibleDC(hcomp);
		m_dc.SelectBitmap(bmp_handle2);
		hcomp.BitBlt(6, 0, rect.right, rect.bottom, m_dc, 0, 0, SRCCOPY);


		// RED arrows
		brush = CreateSolidBrush(COLOR_SPACE_BORDER);
		pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));

		POINT pt[] = {
			rect.right - 1, 255 - ((int)(green)-4),
			rect.right - 1, 255 - ((int)(green)+4),
			rect.right - 5, 255 - ((int)(green)),
			rect.right - 1, 255 - ((int)(green)-4)
		};
		CRgn rgn;
		rgn.CreatePolygonRgn(pt, 4, WINDING);
		hcomp.FillRgn(rgn, brush);
		rtSilder.SetRect(rect.left + 5, 255 - ((int)(green)+1), rect.right - 5, 255 - ((int)green - 2));
		hcomp.SelectPen(pen);
		hcomp.SelectBrush(brush);
		hcomp.Rectangle(rtSilder);
	}

// RGB B  *****

	if (m_combo.GetCurSel() == 2)
	{
		// draw silder
		OnDrawSilder();
		CDC m_dc = CreateCompatibleDC(hcomp);
		m_dc.SelectBitmap(bmp_handle2);
		hcomp.BitBlt(6, 0, rect.right, rect.bottom, m_dc, 0, 0, SRCCOPY);


		// RED arrows
		brush = CreateSolidBrush(COLOR_SPACE_BORDER);
		pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));

		POINT pt[] = {
			rect.right - 1, 255 - ((int)(blue)-4),
			rect.right - 1, 255 - ((int)(blue)+4),
			rect.right - 5, 255 - ((int)(blue)),
			rect.right - 1, 255 - ((int)(blue)-4)
		};
		CRgn rgn;
		rgn.CreatePolygonRgn(pt, 4, WINDING);
		hcomp.FillRgn(rgn, brush);
		rtSilder.SetRect(rect.left + 5, 255 - ((int)(blue)+1), rect.right - 5, 255 - ((int)blue - 2));
		hcomp.SelectPen(pen);
		hcomp.SelectBrush(brush);
		hcomp.Rectangle(rtSilder);
	}

// HSV H  *****

	if (m_combo.GetCurSel() == 3)
	{
		// draw silder
		OnDrawSilder();
		CDC m_dc = CreateCompatibleDC(hcomp);
		m_dc.SelectBitmap(bmp_handle2);
		hcomp.BitBlt(6, 0, rect.right, rect.bottom, m_dc, 0, 0, SRCCOPY);


		double hue, step;

		step = 359.0 / 255.0;


		hue = (double)GetDlgItemInt(IDC_HUE);
		hue = hue / step;

		// Hue arrows
		brush = CreateSolidBrush(COLOR_SPACE_BORDER);
		pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));

		POINT pt[] = {
			rect.right - 1, 255 - ((int)(hue)-4),
			rect.right - 1, 255 - ((int)(hue)+4),
			rect.right - 5, 255 - ((int)(hue)),
			rect.right - 1, 255 - ((int)(hue)-4)
		};
		CRgn rgn;
		rgn.CreatePolygonRgn(pt, 4, WINDING);
		hcomp.FillRgn(rgn, brush);
		rtSilder.SetRect(rect.left + 5, 255 - ((int)(hue)+1), rect.right - 5, 255 - ((int)hue - 2));
		hcomp.SelectPen(pen);
		hcomp.SelectBrush(brush);
		hcomp.Rectangle(rtSilder);
	}

// HSV S  *****
	if (m_combo.GetCurSel() == 4)
	{
		// draw silder
		OnDrawSilder();
		CDC m_dc = CreateCompatibleDC(hcomp);
		m_dc.SelectBitmap(bmp_handle2);
		hcomp.BitBlt(6, 0, rect.right, rect.bottom, m_dc, 0, 0, SRCCOPY);

		double sat, step;

		sat = 0.0;
		step = 100.0 / 255.0;


		sat = (double)GetDlgItemInt(IDC_SATURATION);
		sat = sat / step;

		// Hue arrows
		brush = CreateSolidBrush(COLOR_SPACE_BORDER);
		pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));

		POINT pt[] = {
			rect.right - 1, 255 - ((int)(sat)-4),
			rect.right - 1, 255 - ((int)(sat)+4),
			rect.right - 5, 255 - ((int)(sat)),
			rect.right - 1, 255 - ((int)(sat)-4)
		};
		CRgn rgn;
		rgn.CreatePolygonRgn(pt, 4, WINDING);
		hcomp.FillRgn(rgn, brush);
		rtSilder.SetRect(rect.left + 5, 255 - ((int)(sat)+1), rect.right - 5, 255 - ((int)sat - 2));
		hcomp.SelectPen(pen);
		hcomp.SelectBrush(brush);
		hcomp.Rectangle(rtSilder);
	}

// HSV V  *****
	if (m_combo.GetCurSel() == 5)
	{
		// draw silder
		OnDrawSilder();
		CDC m_dc = CreateCompatibleDC(hcomp);
		m_dc.SelectBitmap(bmp_handle2);
		hcomp.BitBlt(6, 0, rect.right, rect.bottom, m_dc, 0, 0, SRCCOPY);

		double val, step;

		val = 0.0;
		step = 100.0 / 255.0;

		val = (double)GetDlgItemInt(IDC_VALUE);
		val = val / step;

		// Hue arrows
		brush = CreateSolidBrush(COLOR_SPACE_BORDER);
		pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));

		POINT pt[] = {
			rect.right - 1, 255 - ((int)(val)-4),
			rect.right - 1, 255 - ((int)(val)+4),
			rect.right - 5, 255 - ((int)(val)),
			rect.right - 1, 255 - ((int)(val)-4)
		};
		CRgn rgn;
		rgn.CreatePolygonRgn(pt, 4, WINDING);
		hcomp.FillRgn(rgn, brush);
		rtSilder.SetRect(rect.left + 5, 255 - ((int)(val)+1), rect.right - 5, 255 - ((int)val - 2));
		hcomp.SelectPen(pen);
		hcomp.SelectBrush(brush);
		hcomp.Rectangle(rtSilder);
	}

	// color2 border
	rtBor = rect;
	pen = CreatePen(PS_SOLID, 1, COLOR_SPACE_BORDER);
	hcomp.SelectPen(pen);
	hcomp.SelectStockBrush(NULL_BRUSH);
	rtBor.DeflateRect(5, 0, 5, 2);
	hcomp.Rectangle(rtBor);

	hdc.BitBlt(0, 0, rect.right, rect.bottom, hcomp, 0, 0, SRCCOPY);


// Alpha Slider  *****

	hWndAlpha = GetDlgItem(IDC_PALPHA);
	hdc = ::GetDC(hWndAlpha);
	hcomp = CreateCompatibleDC(hdc);

	::GetClientRect(hWndAlpha, &rect);
	hbmp = CreateCompatibleBitmap(hdc, rect.right, 255);
	hcomp.SelectBitmap(hbmp);

	brush = CreateSolidBrush(COLOR_DLG_BK2);
	hcomp.FillRect(&rect, brush);

	// draw silder
	DrawAlpha();
	CDC a_dc = CreateCompatibleDC(hcomp);
	a_dc.SelectBitmap(bmp_handle3);
	hcomp.BitBlt(6, 0, rect.right, rect.bottom, a_dc, 0, 0, SRCCOPY);

	int a;

	a = (int)((float)alpha / 255.0f*255.0f);
	brush = CreateSolidBrush(COLOR_SPACE_BORDER);
	pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));

	POINT pt[] = {
		rect.right - 2, 255 - ((int)(a)-4),
		rect.right - 2, 255 - ((int)(a)+4),
		rect.right - 6, 255 - ((int)(a)),
		rect.right - 2, 255 - ((int)(a)-4)
	};
	CRgn rgn;
	rgn.CreatePolygonRgn(pt, 4, WINDING);
	hcomp.FillRgn(rgn, brush);
	rtSilder.SetRect(rect.left + 5, 255 - ((int)(a)+1), rect.right - 6, 255 - ((int)a - 2));
	hcomp.SelectPen(pen);
	hcomp.SelectBrush(brush);
	hcomp.Rectangle(rtSilder);

	rtBor = rect;
	pen = CreatePen(PS_SOLID, 1, COLOR_SPACE_BORDER);
	hcomp.SelectPen(pen);
	hcomp.SelectStockBrush(NULL_BRUSH);
	rtBor.DeflateRect(5, 0, 6, 2);
	hcomp.Rectangle(rtBor);

	hdc.BitBlt(0, 0, rect.right, rect.bottom, hcomp, 0, 0, SRCCOPY);


//// Current color  *****
	hWndsel = GetDlgItem(IDC_PSELECTCL);
	hdc = ::GetDC(hWndsel);

	::GetClientRect(hWndsel, rect);
	hdc.FillSolidRect(rect, RGB(red, green, blue));


// Dropper ico  *****
	hWndDropper = GetDlgItem(IDC_PSELECTOLD);
	::GetWindowRect(hWndDropper, rtPer);
	ScreenToClient(rtPer);

	Gdiplus::Graphics gr(dc);
	gr.DrawImage(m_DropPng, rtPer.left, rtPer.top);

	hdc.DeleteDC();
	hcomp.DeleteDC();
	brush.DeleteObject();
	pen.DeleteObject();

	dc.SetBkMode(TRANSPARENT);
	dc.SelectFont(GetFont());
	dc.SetTextColor(COLOR_DLG_TEXT);
	if (m_bText)
		dc.DrawText(TEXT("Choose color - 已复制到剪贴板"), -1, CRect(5, 8, 180, 32), DT_LEFT | DT_TOP | DT_SINGLELINE);
	else
		dc.DrawText(TEXT("Choose color"), -1, CRect(5, 8, 180, 32), DT_LEFT | DT_TOP | DT_SINGLELINE);

	return TRUE;
}


LRESULT CMainDlg::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt = { LOWORD(lParam), HIWORD(lParam) };
	ScreenToClient(&pt);
	CRect rect;
	GetClientRect(rect);
	rect.bottom = m_ncHeight;
	return rect.PtInRect(pt) ? HTCAPTION : TRUE;
}

LRESULT CMainDlg::OnCtlColorStatic(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HDC hdc = (HDC)wParam;
	//HWND hwnd = (HWND)lParam;
	HBRUSH hbr = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	//CDC dc(hdc);
	::SetTextColor(hdc, COLOR_DLG_TEXT);
	::SetBkMode(hdc, TRANSPARENT);
	return (LRESULT)hbr;
}

LRESULT CMainDlg::OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return TRUE;
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	if (bmp_handle2) ::DeleteObject(bmp_handle2);
	if (bmp_handle3) ::DeleteObject(bmp_handle3);
	if (row_buffer)  delete[] row_buffer;

	return 0;
}


LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnAbout(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	//m_dAboutDlg.DoModal();

	return 0;
}

LRESULT CMainDlg::OnBnClickedClose(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnBnClickedMin(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ShowWindow(SW_MINIMIZE);
	return 0;
}

void CMainDlg::OnComboBoxSelectChange(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	if (IDC_COMBOCOLOR == nID) SendMessage(WM_PAINT, 0, 0);
	if (IDC_COMMENU == nID)
	{
		if (m_combomenu.GetCurSel() == 0)
			m_dAboutDlg.DoModal();
	}
}

void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}


void CMainDlg::DoDrawRGB()
{
	int		i;		// loop counters
	int		j;

	int		nSkip;	// number of pixels to skip after current row and before next one

	DWORD	*p;		// pointer to first pixel from the blend & a loop pointer

	int		width = 255;
	int		height = 255;
	int		blend_width = width;
	int		blend_height = height;

	// left side components, extended with int_extend bits
	int		left_red, left_green, left_blue;
	// left side component advance, extended with int_extend bits
	int		left_red_adv, left_green_adv, left_blue_adv;
	int		right_red, right_green, right_blue;
	int		right_red_adv, right_green_adv, right_blue_adv;

	// current components, extended with int_extend bits
	int		red, green, blue;
	// advance, extended with int_extend bits
	int		red_adv, green_adv, blue_adv;

	COLORREF	lefttop, righttop, leftbottom, rightbottom;

	p = bmp_data;
	nSkip = 0;

	DoCalculateColors(lefttop, righttop, leftbottom, rightbottom);

	left_red = scaled_red(leftbottom);
	left_green = scaled_green(leftbottom);
	left_blue = scaled_blue(leftbottom);
	left_red_adv = (scaled_red(lefttop) - left_red) / (blend_height);
	left_green_adv = (scaled_green(lefttop) - left_green) / (blend_height);
	left_blue_adv = (scaled_blue(lefttop) - left_blue) / (blend_height);

	right_red = scaled_red(rightbottom);
	right_green = scaled_green(rightbottom);
	right_blue = scaled_blue(rightbottom);
	right_red_adv = (scaled_red(righttop) - right_red) / (blend_height);
	right_green_adv = (scaled_green(righttop) - right_green) / (blend_height);
	right_blue_adv = (scaled_blue(righttop) - right_blue) / (blend_height);

	// outer loop - rows
	i = blend_height;
	while (i--)
	{
		red = left_red;
		green = left_green;
		blue = left_blue;
		red_adv = (right_red - red) / (blend_width);
		green_adv = (right_green - green) / (blend_width);
		blue_adv = (right_blue - blue) / (blend_width);

		// inner loop - pixels @ each row
		j = blend_width;
		while (j--)
		{
			// in DIB bitmap values are 0BGR
			*p++ = RGB(
				blue >> int_extend,
				green >> int_extend,
				red >> int_extend);

			// advance to the next pixel
			red += red_adv; green += green_adv; blue += blue_adv;
		}

		// advance to the next row
		left_red += left_red_adv;
		left_green += left_green_adv;
		left_blue += left_blue_adv;
		right_red += right_red_adv;
		right_green += right_green_adv;
		right_blue += right_blue_adv;
		p += nSkip;
	}
}

void CMainDlg::DoCalculateColors(COLORREF &lefttop, COLORREF &righttop, COLORREF &leftbottom, COLORREF &rightbottom)
{
	// additional_component => edit int
	int red = GetDlgItemInt(IDC_RED);
	int green = GetDlgItemInt(IDC_GREEN);
	int blue = GetDlgItemInt(IDC_BLUE);

	switch (m_combo.GetCurSel())
	{
		case	0:  // red
			lefttop = RGB(red, 0, 0);
			righttop = RGB(red, 255, 0);
			leftbottom = RGB(red, 0, 255);
			rightbottom = RGB(red, 255, 255);
			break;

		case	1:  // green
			lefttop = RGB(0, green, 0);
			righttop = RGB(255, green, 0);
			leftbottom = RGB(0, green, 255);
			rightbottom = RGB(255, green, 255);
			break;

		case	2:  // blue
			lefttop = RGB(0, 0, blue);
			righttop = RGB(0, 255, blue);
			leftbottom = RGB(255, 0, blue);
			rightbottom = RGB(255, 255, blue);
			break;

		default: ATLASSERT(FALSE); break; return;
	}
}

void CMainDlg::DoDrawHSV_Hue(void)
{
	CRect rect;
	HWND hWndCo = GetDlgItem(IDC_PCOLOR);
	::GetWindowRect(hWndCo, &rect);
	ScreenToClient(rect);
	int		i;		// loop counters

	DWORD	*p;		// pointer to first pixel from the blend & a loop pointer

	int		width = 255;		// rect.Width();
	int		height = 255;		// rect.Height();
	int		blend_width = width;  // rect.Width();
	int		blend_height = height; // rect.Height();

	double	hue;
	double	val, val_adv;


	p = bmp_data;

	// constant
	hue = GetDlgItemInt(IDC_HUE);

	// outer loop - rows
	i = blend_height;

	// initial and change
	val = 0.0;
	val_adv = 1.0 / i;

	while (i--)
	{
		GTDrawHelper m_Panel;
		m_Panel.HSV_SAT(p, blend_width, hue, val);
		val += val_adv;
		p += blend_width;
	}
}

void CMainDlg::DoDrawHSV_Sat()
{
	int		i;		// loop counters

	int		nSkip;	// number of pixels to skip after current row and before next one

	DWORD	*p;		// pointer to first pixel from the blend & a loop pointer


	int		width = 255;
	int		height = 255;
	int		blend_width = width;
	int		blend_height = height;

	double	sat;
	double	val, val_adv;

	p = bmp_data;
	nSkip = 0;

	// constant
	sat = (double)(GetDlgItemInt(IDC_SATURATION) * 0.01);

	// outer loop - rows
	i = blend_height;

	// initial and change
	val = 0.0;
	val_adv = 1.0 / i;// HSV_LOOP_STEPS(i);

	while (i--)
	{
		GTDrawHelper m_Panel;
		m_Panel.HSV_HUE(p, blend_width, sat, val);
		val += val_adv;
		p += (blend_width + nSkip);
	}
}

void CMainDlg::DoDrawHSV_Val()
{
	int		i;		// loop counters

	int		nSkip;	// number of pixels to skip after current row and before next one

	DWORD	*p;		// pointer to first pixel from the blend & a loop pointer

	int		width = 255;
	int		height = 255;
	int		blend_width = width;
	int		blend_height = height;

	double	sat;
	double	sat_adv;
	double	val;

	p = bmp_data;
	nSkip = 0;

	// constant
	val = (double)(GetDlgItemInt(IDC_VALUE) * 0.01);

	// outer loop - rows
	i = blend_height;
	sat = 0.0;
	sat_adv = 1.0 / i; //HSV_LOOP_STEPS(i);

	while (i--)
	{
		GTDrawHelper m_Panel;
		m_Panel.HSV_HUE(p, blend_width, sat, val);
		sat += sat_adv;
		p += (blend_width + nSkip);
	}
}

void CMainDlg::OnDrawSilder()
{
	CRect rtSil;
	HWND hSil = GetDlgItem(IDC_PSILDER);
	::GetWindowRect(hSil, rtSil);

	switch (m_combo.GetCurSel())
	{
		// 0 1 2 3 4 5
		// r g b h s v
		case	0:
		case	1:
		case	2:		DrawRGB_ALL();	 break;
		case	3:		DrawHSV_H();	 break;
		case	4:		DrawHSV_S();     break;
		case	5:		DrawHSV_V();	 break;

		default: ATLASSERT(FALSE); return;
	}

	int	i;

	int		nSkip;	// number of pixels to skip after current row and before next one

	DWORD	*p;		// loop pointers

	int		width = rtSil.Width() - 12;
	int		height = 255;
	int		blend_width = width;
	int		blend_height = height;

	// prepare
	p = bmp_data2;
	nSkip = 0;


	DWORD	*source = row_buffer;

	i = blend_height;
	while (i--)
	{
		GTDrawHelper m_Panel;
		m_Panel.set(&p, *source++, blend_width), p += nSkip;
	}
}

void CMainDlg::DrawRGB_ALL()
{
	COLORREF	start_color, end_color;
	int	red, green, blue;

	red = GetDlgItemInt(IDC_RED);
	green = GetDlgItemInt(IDC_GREEN);
	blue = GetDlgItemInt(IDC_BLUE);

	switch (m_combo.GetCurSel())
	{
		case	0:  // red
			start_color = RGB(0, green, blue);
			end_color = RGB(255, green, blue);
			break;

		case	1:  // green
			start_color = RGB(red, 0, blue);
			end_color = RGB(red, 255, blue);
			break;

		case	2:  // blue
			start_color = RGB(red, green, 0);
			end_color = RGB(red, green, 255);
			break;

		default: ATLASSERT(FALSE); return;
	}

	GTDrawHelper m_Panel;
	m_Panel.GetRGB(row_buffer, m_nBuffSize, start_color, end_color);
}

void CMainDlg::DrawHSV_H()
{
	GTDrawHelper m_Panel;
	m_Panel.HSV_HUE(row_buffer, m_nBuffSize, 1, 1);
}

void CMainDlg::DrawHSV_S()
{
	int hue = GetDlgItemInt(IDC_HUE);
	double value = (double)(GetDlgItemInt(IDC_VALUE) * 0.01);
	GTDrawHelper m_Panel;
	m_Panel.HSV_SAT(row_buffer, m_nBuffSize, hue, value);
}


void CMainDlg::DrawHSV_V()
{
	int hue = GetDlgItemInt(IDC_HUE);
	double sat = (double)(GetDlgItemInt(IDC_SATURATION) * 0.01);
	GTDrawHelper m_Panel;
	m_Panel.HSV_VAL(row_buffer, m_nBuffSize, hue, sat);
}

void CMainDlg::DrawAlpha()
{
	CRect rtA;
	HWND hA = GetDlgItem(IDC_PALPHA);
	::GetWindowRect(hA, rtA);

	int red, green, blue, alpha;

	red = GetDlgItemInt(IDC_RED);
	green = GetDlgItemInt(IDC_GREEN);
	blue = GetDlgItemInt(IDC_BLUE);
	alpha = GetDlgItemInt(IDC_ALPHA);

	float rf = (float)red / 255.0f;
	float gf = (float)green / 255.0f;
	float bf = (float)blue / 255.0f;

	int	w = rtA.Width() - 12;
	int	h = 256;

	int	j = h;
	int	i = h;

	DWORD *p = bmp_data3;

	BOOL flag = FALSE;
	while (i--)
	{
		COLORREF cb, cw;

		if (!(i % (rtA.Width() / 2 - 6)))
			flag = !flag;

		float af = 1.0f - (float)i / 255.0f;
		DWORD *pixel = p;

		// step
		cb = RGB((bf*af) * 255, (gf*af) * 255, (rf*af) * 255);
		cw = RGB((bf*af + 1 - af) * 255, (gf*af + 1 - af) * 255, (rf*af + 1 - af) * 255);

		if (flag)
		{
			for (int x = 6; x < (rtA.Width() / 2); x++)
				bmp_data3[((h - i)*w + x)] = cw;
			for (int x = (rtA.Width() / 2); x < rtA.Width() * 4; x++)
				bmp_data3[((h - i)*w + x)] = cb;
		}
		else
		{
			for (int x = 6; x < (rtA.Width() / 2); x++)
				bmp_data3[((h - i)*w + x)] = cb;
			for (int x = (rtA.Width() / 2); x < rtA.Width() * 4; x++)
				bmp_data3[((h - i)*w + x)] = cw;
		}
	}
}

// Color Picker start

void CMainDlg::SetRGB(unsigned short r, unsigned short g, unsigned short b)
{
	CurrCol.r = min(r, 255);
	CurrCol.g = min(g, 255);
	CurrCol.b = min(b, 255);

	CurrCol.UpdateHSV();
}

void CMainDlg::SetHSV(unsigned short h, unsigned short s, unsigned short v)
{
	CurrCol.h = min(h, 359);
	CurrCol.s = min(s, 100);
	CurrCol.v = min(v, 100);

	CurrCol.UpdateRGB();
}

void CMainDlg::SetAlpha(unsigned short a)
{
	CurrCol.a = min(a, 255);
}

// RGB 转 HSL
void CMainDlg::SetHSL(unsigned short h, unsigned short s, unsigned short l)
{
	CurrCol.UpdateHSL();
}

void CMainDlg::SetRGB_F_HSL(unsigned short h, unsigned short s, unsigned short l)
{
	CurrCol.h1 = min(h, 359);
	CurrCol.s1 = min(s, 100);
	CurrCol.l  = min(l, 100);

	CurrCol.RGB_F_HSL(CurrCol.h1, CurrCol.s1, CurrCol.l);
}

void CMainDlg::SetRGB_F_HTML()
{
	CString str;
	m_eHtml.GetWindowText(str);

	CurrCol.RGB_F_HTML(str);
}

void CMainDlg::SetRGB_F_ASS()
{
	CString str;
	m_eAss.GetWindowText(str);

	CurrCol.RGB_F_ASS(str);
}

SColour CMainDlg::GetCurrentColour()
{
	return CurrCol;
}

// HSV 转 RGB
void SColour::UpdateRGB()
{
	int conv;
	double hue, sat, val;
	int base;

	hue = (float)h / 100.0f;
	sat = (float)s / 100.0f;
	val = (float)v / 100.0f;

	if ((float)s == 0) // Acromatic color (gray). Hue doesn't mind.
	{
		conv = (unsigned short)(255.0f * val);
		r = b = g = conv;
		return;
	}

	base = (unsigned short)(255.0f * (1.0 - sat) * val);

	switch ((unsigned short)((float)h / 60.0f))
	{
		case 0:
			r = (unsigned short)(255.0f * val);
			g = (unsigned short)((255.0f * val - base) * (h / 60.0f) + base);
			b = base;
			break;

		case 1:
			r = (unsigned short)((255.0f * val - base) * (1.0f - ((h % 60) / 60.0f)) + base);
			g = (unsigned short)(255.0f * val);
			b = base;
			break;

		case 2:
			r = base;
			g = (unsigned short)(255.0f * val);
			b = (unsigned short)((255.0f * val - base) * ((h % 60) / 60.0f) + base);
			break;

		case 3:
			r = base;
			g = (unsigned short)((255.0f * val - base) * (1.0f - ((h % 60) / 60.0f)) + base);
			b = (unsigned short)(255.0f * val);
			break;

		case 4:
			r = (unsigned short)((255.0f * val - base) * ((h % 60) / 60.0f) + base);
			g = base;
			b = (unsigned short)(255.0f * val);
			break;

		case 5:
			r = (unsigned short)(255.0f * val);
			g = base;
			b = (unsigned short)((255.0f * val - base) * (1.0f - ((h % 60) / 60.0f)) + base);
			break;
	}
}

// RGB 转 HSV
void SColour::UpdateHSV()
{
	unsigned short max, min, delta;
	short temp;

	max = MAX(r, g, b);
	min = MIN(r, g, b);
	delta = max - min;

	if (max == 0)
	{
		s = h = v = 0;
		return;
	}

	v = (unsigned short)((double)max / 255.0*100.0);
	s = (unsigned short)(((double)delta / max)*100.0);

	if (r == max)
		temp = (short)(60 * ((g - b) / (double)delta));
	else if (g == max)
		temp = (short)(60 * (2.0 + (b - r) / (double)delta));
	else
		temp = (short)(60 * (4.0 + (r - g) / (double)delta));

	if (temp < 0)
		h = temp + 360;
	else
		h = temp;
}

// RGB 转 HSL
void SColour::UpdateHSL()
{

	double R, G, B, Max, Min, del_R, del_G, del_B, del_Max, H, S, L;
	R = r / 255.0;       //Where RGB values = 0 ÷ 255
	G = g / 255.0;
	B = b / 255.0;

	Min = min(R, min(G, B));    //Min. value of RGB
	Max = max(R, max(G, B));    //Max. value of RGB
	del_Max = Max - Min;        //Delta RGB value

	L = (Max + Min) / 2.0;

	if (del_Max == 0)           //This is a gray, no chroma...
	{
		H = 0;                  //HSL results = 0 ÷ 1
		S = 0;
	}
	else                        //Chromatic data...
	{
		if (L < 0.5) S = del_Max / (Max + Min);
		else         S = del_Max / (2.0 - Max - Min);

		del_R = ((Max - R) / 6.0) + (del_Max / 2.0) / del_Max;
		del_G = ((Max - G) / 6.0) + (del_Max / 2.0) / del_Max;
		del_B = ((Max - B) / 6.0) + (del_Max / 2.0) / del_Max;

		if (R == Max) H = (del_B - del_G);
		else if (G == Max) H = (1.0 / 3.0) + del_R - del_B;
		else if (B == Max) H = (2.0 / 3.0) + del_G - del_R;

		if (H < 0)  H += 1;
		if (H > 1)  H -= 1;
	}
	h1 = (unsigned short)(H * 360.0);
	s1 = (unsigned short)(S * 100.0);
	l = (unsigned short)(L * 100.0);
}

// HSL转RGB
void SColour::RGB_F_HSL(int h, int s, int l)
{
	double H, S, L;
	double var_1, var_2;

	H = h / 360.0f;
	S = s / 100.0f;
	L = l / 100.0f;

	if (S == 0)							 //HSL values = 0 ÷ 1
	{
		r = (unsigned short)(L * 255.0);                   //RGB results = 0 ÷ 255
		g = (unsigned short)(L * 255.0);
		b = (unsigned short)(L * 255.0);
	}
	else
	{
		if (L < 0.5) var_2 = L * (1 + S);
		else         var_2 = (L + S) - (S * L);

		var_1 = 2.0 * L - var_2;

		r = (unsigned short)(255.0 * HSL2RGB(var_1, var_2, H + (1.0 / 3.0)));
		g = (unsigned short)(255.0 * HSL2RGB(var_1, var_2, H));
		b = (unsigned short)(255.0 * HSL2RGB(var_1, var_2, H - (1.0 / 3.0)));
	}
}

// hsl helper func
double SColour::HSL2RGB(double v1, double v2, double vH)
{
	if (vH < 0) vH += 1;
	if (vH > 1) vH -= 1;
	if (6.0 * vH < 1) return v1 + (v2 - v1) * 6.0 * vH;
	if (2.0 * vH < 1) return v2;
	if (3.0 * vH < 2) return v1 + (v2 - v1) * ((2.0 / 3.0) - vH) * 6.0;
	return (v1);
}

// HTML转RGB
void SColour::RGB_F_HTML(CString szValue)
{
	std::string hex = CW2A(szValue.GetString());

	int* rgb = new int[3];
	std::stringstream ss;
	std::string str;

	// HTML
	if (hex[0] == '#') {
		hex.erase(0, 1);
	}

	int size = hex.size();

	for (int i = 0; i < 3; i++) {
		// Determine 3 or 6 character format.
		if (size == 3) { str = std::string(2, hex[i]); }
		else if (size == 6) { str = hex.substr(i * 2, 2); }
		else { break; }

		ss << std::hex << str;
		ss >> rgb[i];
		ss.clear();
	}
	r = rgb[0];
	g = rgb[1];
	b = rgb[2];
	//ATLTRACE(L"%d %d %d\n", r, g, b);
}

// ASS转RGB
void SColour::RGB_F_ASS(CString szValue)
{
	szValue.Remove('&');
	szValue.Remove('H');
	std::string hex = CW2A(szValue.GetString());

	int* rgb = new int[3];
	std::stringstream ss;
	std::string str;
	
	int size = hex.size();

	for (int i = 0; i < 3; i++) {
		// Determine 3 or 6 character format.
		if (size == 3) { str = std::string(2, hex[i]); }
		else if (size == 6) { str = hex.substr(i * 2, 2); }
		else { break; }

		ss << std::hex << str;
		ss >> rgb[i];
		ss.clear();
	}
	r = rgb[2];
	g = rgb[1];
	b = rgb[0];
	//ATLTRACE(L"%d %d %d\n", b, g, r);

}


// Main

void CMainDlg::UpdateValues(SColour col)
{
	CString str;
	SetDlgItemInt(IDC_RED, col.r);
	SetDlgItemInt(IDC_GREEN, col.g);
	SetDlgItemInt(IDC_BLUE, col.b);
	SetDlgItemInt(IDC_ALPHA, col.a);
	SetDlgItemInt(IDC_HUE, col.h);
	SetDlgItemInt(IDC_SATURATION, col.s);
	SetDlgItemInt(IDC_VALUE, col.v);
	SetDlgItemInt(IDC_HEEL, col.h1);
	SetDlgItemInt(IDC_SSTA, col.s1);
	SetDlgItemInt(IDC_SLIGHT, col.l);

	str.Format(TEXT("#%02X%02X%02X"), col.r, col.g, col.b);
	SetDlgItemText(IDC_HTML, str);
	str.Format(TEXT("&H%02X%02X%02X&"), col.b, col.g, col.r);
	SetDlgItemText(IDC_ASS, str);
	

	SetSel2(IDC_RED);
	SetSel2(IDC_GREEN);
	SetSel2(IDC_BLUE);  	// r g b
	SetSel2(IDC_ALPHA);     // alpha
	SetSel2(IDC_HEEL);
	SetSel2(IDC_SSTA);
	SetSel2(IDC_SLIGHT);	// h s l
	SetSel2(IDC_HUE);
	SetSel2(IDC_SATURATION);
	SetSel2(IDC_VALUE);		// h s v
	SetSel2(IDC_HTML);
	SetSel2(IDC_ASS);
}

// 处理光标焦点
void CMainDlg::SetSel2(int nID)
{
	CPoint p;
	GetCaretPos(&p);
	HWND hwnd = GetDlgItem(nID);

	DWORD dwRet = (DWORD)::SendMessage(hwnd, EM_CHARFROMPOS, 0, MAKELPARAM(p.x, p.y));
	// 当前字符索引
	int i = (int)((short)LOWORD(dwRet)); 
	if (GetDlgItemInt(nID) == 0)	    i = 1;
	if (GetDlgItemInt(nID) / 10  == 1)  i = 2;
	if (GetDlgItemInt(nID) / 100 == 1)  i = 3;
	::SendMessage(hwnd, EM_SETSEL, i, i);

	//CPoint p;
	//GetCaretPos(&p);
	//int i = m_eRed.CharFromPos(p);
	//m_eRed.SetSel(i, i);

	//CString s;
	//s.Format(_T("Point = %ld\n"), i);
	//ATLTRACE(s);

	//int nLineIndex = HIWORD(i);
	//int nCharIndex = LOWORD(i);
	//ATLTRACE(L"%d %d\n", nLineIndex, nCharIndex);
}

void CMainDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rectC1, rectC2, rectA, rtPer;
	HWND Color1, Color2, Alpha, DropPer;

	Color1 = GetDlgItem(IDC_PCOLOR);
	Color2 = GetDlgItem(IDC_PSILDER);
	Alpha = GetDlgItem(IDC_PALPHA);	
	DropPer = GetDlgItem(IDC_PSELECTOLD);

	::GetWindowRect(Color1, &rectC1);
	::GetWindowRect(Color2, &rectC2);
	::GetWindowRect(Alpha, &rectA);
	::GetWindowRect(DropPer, rtPer);
	ClientToScreen(&point);

	// 隐藏edit光标
	::HideCaret(::GetFocus());
	::SetFocus(WindowFromPoint(point));

	// color1
	if (rectC1.PtInRect(point))
	{
		m_bColor = TRUE;
		m_bHue = FALSE;
		m_bAlpha = FALSE;
	}

	// color2
	if (rectC2.PtInRect(point))
	{
		m_bHue = TRUE;
		m_bColor = FALSE;
		m_bAlpha = FALSE;
	}

	// alpha
	if (rectA.PtInRect(point))
	{
		m_bAlpha = TRUE;
		m_bColor = FALSE;
		m_bHue = FALSE;
	}

	if (rtPer.PtInRect(point))
	{
		m_drop.ShowWindow(SW_MAXIMIZE);
		
		m_selcol.m_bShow = TRUE;
		SendMessage(m_selcol.m_hWnd, WM_STARTTIMER, 0, 0);
	}


	ColorFromPos();

	// 隐藏光标
	//if (m_bColor || m_bHue || m_bAlpha)
	//{
	//	while (ShowCursor(FALSE) >= 0)
	//		ShowCursor(FALSE);
	//}

}

void CMainDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bColor = FALSE;
	m_bHue = FALSE;
	m_bAlpha = FALSE;
	ReleaseCapture();

	//while (ShowCursor(TRUE) < 0)
	//	ShowCursor(TRUE);

}

void CMainDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (nFlags &MK_LBUTTON)
	{
		this->SetCapture();
		ColorFromPos();
	}
}

void CMainDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CRect rect;
	CString str;

	m_grRgb.GetWindowRect(rect);
	ScreenToClient(rect);
	if (rect.PtInRect(point))
	{
		// 防止快速点击次数过多。。。
		if (!m_bText)
		{
			str.Format(TEXT("%d, %d, %d"),
				GetDlgItemInt(IDC_RED),
				GetDlgItemInt(IDC_GREEN),
				GetDlgItemInt(IDC_BLUE));
			SetClipboardText(str);
			m_bText = TRUE;
			// 刷新文本提示区域
			InvalidateRect(CRect(5, 8, 180, 32), FALSE);
			SetTimer(1, 3500);
		}
	}

	m_grHsl.GetWindowRect(rect);
	ScreenToClient(rect);
	if (rect.PtInRect(point))
	{
		if (!m_bText)
		{
			str.Format(TEXT("%d, %d, %d"),
				GetDlgItemInt(IDC_HEEL),
				GetDlgItemInt(IDC_SSTA),
				GetDlgItemInt(IDC_SLIGHT));
			SetClipboardText(str);
			m_bText = TRUE;
			InvalidateRect(CRect(5, 8, 180, 32), FALSE);
			SetTimer(1, 3500);
		}
	}

	m_grHsv.GetWindowRect(rect);
	ScreenToClient(rect);
	if (rect.PtInRect(point))
	{
		if (!m_bText)
		{
			str.Format(TEXT("%d, %d, %d"),
				GetDlgItemInt(IDC_HUE),
				GetDlgItemInt(IDC_SATURATION),
				GetDlgItemInt(IDC_VALUE));
			SetClipboardText(str);
			m_bText = TRUE;
			InvalidateRect(CRect(5, 8, 180, 32), FALSE);
			SetTimer(1, 3000);
		}
	}
}

void CMainDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (m_bText)
	{
		m_bText = FALSE;
		InvalidateRect(CRect(5, 8, 180, 32), FALSE);
		KillTimer(1);
	}
}

void CMainDlg::OnMove(CPoint ptPos)
{
	CRect rect, rtSil;

	// init choose and normal color wnd
	HWND hSelCol = GetDlgItem(IDC_SELCOLOR);
	::GetWindowRect(hSelCol, rect);
	m_selcol.MoveWindow(rect);
	m_selcol.ShowWindow(SW_SHOW);

	HWND hPicCol = GetDlgItem(IDC_PICCOL);
	::GetWindowRect(hPicCol, rect);
	m_piccol.MoveWindow(rect);
	m_piccol.ShowWindow(SW_SHOW);



	int w = 255; // rect.Width();
	int h = 255; // rect.Height();

	if (bmp_handle)
		::DeleteObject(bmp_handle);

	// color1
	bmp_info.bmiHeader.biSize = sizeof (BITMAPINFO);
	bmp_info.bmiHeader.biWidth = w;
	bmp_info.bmiHeader.biHeight = h;
	bmp_info.bmiHeader.biPlanes = 1;
	bmp_info.bmiHeader.biBitCount = 32;
	bmp_info.bmiHeader.biCompression = BI_RGB;
	bmp_info.bmiHeader.biSizeImage = w * h * 32 / 8;
	bmp_info.bmiHeader.biXPelsPerMeter =
	bmp_info.bmiHeader.biYPelsPerMeter = 72 * 2 * 1000;
	bmp_info.bmiHeader.biClrUsed = 0;
	bmp_info.bmiHeader.biClrImportant = 0;
	bmp_data = NULL;
	bmp_handle = CreateDIBSection(
		NULL, &bmp_info, DIB_RGB_COLORS, (void **)&bmp_data, NULL, 0);


	if (bmp_handle2)
		::DeleteObject(bmp_handle2);

	HWND hSild = GetDlgItem(IDC_PSILDER);
	::GetWindowRect(hSild, rtSil);

	// 留12px绘制滑块
	w = rtSil.Width() - 12;
	h = 256;
	m_nBuffSize = 256;

	// color2
	bmp_info2.bmiHeader.biSize = sizeof (BITMAPINFO);
	bmp_info2.bmiHeader.biWidth = w;
	bmp_info2.bmiHeader.biHeight = h;
	bmp_info2.bmiHeader.biPlanes = 1;
	bmp_info2.bmiHeader.biBitCount = 32;
	bmp_info2.bmiHeader.biCompression = BI_RGB;
	bmp_info2.bmiHeader.biSizeImage = w * h * 32 / 8;
	bmp_info2.bmiHeader.biXPelsPerMeter =
	bmp_info2.bmiHeader.biYPelsPerMeter = 72 * 2 * 1000;
	bmp_info2.bmiHeader.biClrUsed = 0;
	bmp_info2.bmiHeader.biClrImportant = 0;
	bmp_data2 = NULL;
	bmp_handle2 = CreateDIBSection(
		NULL, &bmp_info2, DIB_RGB_COLORS, (void **)&bmp_data2, NULL, 0);

	if (bmp_handle3)
		::DeleteObject(bmp_handle3);


	// alpha
	bmp_info3.bmiHeader.biSize = sizeof (BITMAPINFO);
	bmp_info3.bmiHeader.biWidth = w;
	bmp_info3.bmiHeader.biHeight = h;
	bmp_info3.bmiHeader.biPlanes = 1;
	bmp_info3.bmiHeader.biBitCount = 32;
	bmp_info3.bmiHeader.biCompression = BI_RGB;
	bmp_info3.bmiHeader.biSizeImage = w * h * 32 / 8;
	bmp_info3.bmiHeader.biXPelsPerMeter =
	bmp_info3.bmiHeader.biYPelsPerMeter = 72 * 2 * 1000;
	bmp_info3.bmiHeader.biClrUsed = 0;
	bmp_info3.bmiHeader.biClrImportant = 0;
	bmp_data3 = NULL;
	bmp_handle3 = CreateDIBSection(
		NULL, &bmp_info3, DIB_RGB_COLORS, (void **)&bmp_data3, NULL, 0);

}

void CMainDlg::SetClipboardText(CString str)
{
	if (::OpenClipboard(NULL))
	{
		if (EmptyClipboard())
		{
			size_t cbStr = (str.GetLength() + 1)*sizeof(TCHAR);
			HGLOBAL hData = GlobalAlloc(GMEM_MOVEABLE, cbStr);
			memcpy_s(GlobalLock(hData), cbStr, str.LockBuffer(), cbStr);
			str.UnlockBuffer();
			GlobalUnlock(hData);
			UINT nFormat = (sizeof(TCHAR) == sizeof(WCHAR) ? CF_UNICODETEXT : CF_TEXT);
			if (NULL == ::SetClipboardData(nFormat, hData))
			{
				CloseClipboard();
				return;
			}
		}
		CloseClipboard();
	}
}

void CMainDlg::OnEditControlChange(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	int r, g, b, a;
	int h, s, v;
	int h1, s1, l;

	if (GetDlgItemInt(nID) == 0 && nID != IDC_ASS && nID != IDC_HTML) return;

	if (IDC_RED == nID || IDC_GREEN == nID || IDC_BLUE == nID) {
		
		r = GetDlgItemInt(IDC_RED);
		g = GetDlgItemInt(IDC_GREEN);
		b = GetDlgItemInt(IDC_BLUE);
		SetRGB(r, g, b);
		SetHSL(r, g, b);

		UpdateValues(GetCurrentColour());
		SendMessage(WM_PAINT, 0, 0);
	}
	
	if (IDC_HUE == nID || IDC_SATURATION == nID || IDC_VALUE == nID){

		h = GetDlgItemInt(IDC_HUE);
		s = GetDlgItemInt(IDC_SATURATION);
		v = GetDlgItemInt(IDC_VALUE);
		SetHSV(h, s, v);
		SetHSL(GetCurrentColour().r, GetCurrentColour().g, GetCurrentColour().b);

		UpdateValues(GetCurrentColour());
		SendMessage(WM_PAINT, 0, 0);
	}

	if (IDC_HEEL == nID || IDC_SSTA == nID || IDC_SLIGHT == nID){

		h1 = GetDlgItemInt(IDC_HEEL);
		s1 = GetDlgItemInt(IDC_SSTA);
		l = GetDlgItemInt(IDC_SLIGHT);
		SetRGB_F_HSL(h1, s1, l);
		SetRGB(GetCurrentColour().r, GetCurrentColour().g, GetCurrentColour().b);

		UpdateValues(GetCurrentColour());
		SendMessage(WM_PAINT, 0, 0);
	}

	if (IDC_ALPHA == nID){

		a = GetDlgItemInt(IDC_ALPHA);
		SetAlpha(a);

		UpdateValues(GetCurrentColour());
		SendMessage(WM_PAINT, 0, 0);
	}

	if (IDC_HTML == nID){

		SetRGB_F_HTML();
		SetRGB(GetCurrentColour().r, GetCurrentColour().g, GetCurrentColour().b);
		SetHSL(GetCurrentColour().r, GetCurrentColour().g, GetCurrentColour().b);

		CString str;
		SColour col = GetCurrentColour();

		// 手动更新
		SetDlgItemInt(IDC_RED, col.r);
		SetDlgItemInt(IDC_GREEN, col.g);
		SetDlgItemInt(IDC_BLUE, col.b);
		SetDlgItemInt(IDC_ALPHA, col.a);
		SetDlgItemInt(IDC_HUE, col.h);
		SetDlgItemInt(IDC_SATURATION, col.s);
		SetDlgItemInt(IDC_VALUE, col.v);
		SetDlgItemInt(IDC_HEEL, col.h1);
		SetDlgItemInt(IDC_SSTA, col.s1);
		SetDlgItemInt(IDC_SLIGHT, col.l);

		str.Format(TEXT("&H%02X%02X%02X&"), col.b, col.g, col.r);
		SetDlgItemText(IDC_ASS, str);
		SendMessage(WM_PAINT, 0, 0);
	}

	if (IDC_ASS == nID){

		SetRGB_F_ASS();
		SetRGB(GetCurrentColour().r, GetCurrentColour().g, GetCurrentColour().b);
		SetHSL(GetCurrentColour().r, GetCurrentColour().g, GetCurrentColour().b);

		CString str;
		SColour col = GetCurrentColour();

		// 手动更新
		SetDlgItemInt(IDC_RED, col.r);
		SetDlgItemInt(IDC_GREEN, col.g);
		SetDlgItemInt(IDC_BLUE, col.b);
		SetDlgItemInt(IDC_ALPHA, col.a);
		SetDlgItemInt(IDC_HUE, col.h);
		SetDlgItemInt(IDC_SATURATION, col.s);
		SetDlgItemInt(IDC_VALUE, col.v);
		SetDlgItemInt(IDC_HEEL, col.h1);
		SetDlgItemInt(IDC_SSTA, col.s1);
		SetDlgItemInt(IDC_SLIGHT, col.l);
		str.Format(TEXT("#%02X%02X%02X"), col.r, col.g, col.b);
		SetDlgItemText(IDC_HTML, str);
		SendMessage(WM_PAINT, 0, 0);
	}
}


// 更新取色器获取到的颜色
LRESULT CMainDlg::OnUpdateColor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int nID;
	CString str;

	// choose color
	if ((int)wParam == UPDATE_CHOOSE_COLOR)
	{
		CurrCol.r = GetRValue(lParam);
		CurrCol.g = GetGValue(lParam);
		CurrCol.b = GetBValue(lParam);
		SetRGB(CurrCol.r, CurrCol.g, CurrCol.b);
		SetHSL(CurrCol.r, CurrCol.g, CurrCol.b);
		UpdateValues(GetCurrentColour());
		SendMessage(WM_PAINT, 0, 0);
	}

	// mouse wheel color
	if ((int)wParam == UPDATE_WHEEL_COLOR)
	{
		nID = LOWORD(lParam);
		if (IDC_RED == nID || IDC_GREEN == nID || IDC_BLUE == nID || IDC_ALPHA == nID){
			SetDlgItemInt(nID, HIWORD(lParam));
			OnEditControlChange(NULL, nID, NULL);
		}

		if (IDC_HEEL == nID || IDC_SSTA == nID || IDC_SLIGHT == nID){
			SetDlgItemInt(nID, HIWORD(lParam));
			OnEditControlChange(NULL, nID, NULL);
		}

		if (IDC_HUE == nID || IDC_SATURATION == nID || IDC_VALUE == nID){
			SetDlgItemInt(nID, HIWORD(lParam));
			OnEditControlChange(NULL, nID, NULL);
		}
	}
	return TRUE;
}


void CMainDlg::ColorFromPos()
{
	CPoint p;
	CRect rectC1, rectC2, rectA;
	HWND Color1, Color2, Alpha;
	HWND hDlg = m_hWnd;

	Color1 = GetDlgItem(IDC_PCOLOR);
	Color2 = GetDlgItem(IDC_PSILDER);
	Alpha = GetDlgItem(IDC_PALPHA);

	::GetWindowRect(Color1, &rectC1);
	::GetWindowRect(Color2, &rectC2);
	::GetWindowRect(Alpha, &rectA);

	::GetCursorPos(&p);


	// Color1
	if (m_bColor)
	{
		int x = (p.x - rectC1.left);
		int y = (p.y - rectC1.top);
		CLAMP(x, 0, 255);
		CLAMP(y, 0, 255);

		// RGB R
		if (m_combo.GetCurSel() == 0)
		{
			SetRGB((unsigned short)GetDlgItemInt(IDC_RED), x, y);
			SetHSL(0, 0, 0);
			UpdateValues(GetCurrentColour());
		}

		// RGB G
		if (m_combo.GetCurSel() == 1)
		{
			SetRGB((unsigned short)(x),
				(unsigned short)GetDlgItemInt(IDC_GREEN),
				(unsigned short)(y));

			SetHSL(0, 0, 0);
			UpdateValues(GetCurrentColour());
		}

		// RGB B
		if (m_combo.GetCurSel() == 2)
		{
			SetRGB(y, x, (unsigned short)GetDlgItemInt(IDC_BLUE));
			SetHSL(0, 0, 0);
			UpdateValues(GetCurrentColour());
		}


		// HSV H
		if (m_combo.GetCurSel() == 3)
		{
			SetHSV((unsigned short)GetDlgItemInt(IDC_HUE),
				(unsigned short)(x / 255.0*100.0),
				(unsigned short)((255 - y) / 255.0*100.0));
			SetHSL(0,0,0);

			UpdateValues(GetCurrentColour());
		}

		// HSV S
		if (m_combo.GetCurSel() == 4)
		{
			SetHSV((unsigned short)(x / 255.0*359.0),
				(unsigned short)GetDlgItemInt(IDC_SATURATION),
				(unsigned short)((255 - y) / 255.0*100.0));
			SetHSL(0, 0, 0);
			UpdateValues(GetCurrentColour());
		}

		// HSV V
		if (m_combo.GetCurSel() == 5)
		{
			SetHSV((unsigned short)(x / 255.0*359.0),
				(unsigned short)((255 - y) / 255.0*100.0),
				(unsigned short)GetDlgItemInt(IDC_VALUE));
			SetHSL(0, 0, 0);
			UpdateValues(GetCurrentColour());
		}
	}

	// Color2
	if (m_bHue)
	{
		int y = (p.y - rectC2.top);
		CLAMP(y, 0, 255);

		// RGB R
		if (m_combo.GetCurSel() == 0)
		{
			SetRGB((unsigned short)(255 - y),
				(unsigned short)GetDlgItemInt(IDC_GREEN),
				(unsigned short)GetDlgItemInt(IDC_BLUE));
			SetHSL(0, 0, 0);

			UpdateValues(GetCurrentColour());
		}

		// RGB G
		if (m_combo.GetCurSel() == 1)
		{
			SetRGB((unsigned short)GetDlgItemInt(IDC_RED),
				(unsigned short)(255 - y),
				(unsigned short)GetDlgItemInt(IDC_BLUE));
			SetHSL(0, 0, 0);

			UpdateValues(GetCurrentColour());
		}

		// RGB B
		if (m_combo.GetCurSel() == 2)
		{
			SetRGB((unsigned short)GetDlgItemInt(IDC_RED),
				(unsigned short)GetDlgItemInt(IDC_GREEN),
				(unsigned short)(255 - y));
			SetHSL(0, 0, 0);

			UpdateValues(GetCurrentColour());
		}

		// HSV H
		if (m_combo.GetCurSel() == 3)
		{
			SetHSV((unsigned short)((255 - y) / 255.0*359.0),
				(unsigned short)GetDlgItemInt(IDC_SATURATION),
				(unsigned short)GetDlgItemInt(IDC_VALUE));
			SetHSL(0,0,0);
			UpdateValues(GetCurrentColour());
		}

		// HSV S
		if (m_combo.GetCurSel() == 4)
		{
			SetHSV((unsigned short)GetDlgItemInt(IDC_HUE),
				(unsigned short)((255 - y) / 255.0*100.0),
				(unsigned short)GetDlgItemInt(IDC_VALUE));
			SetHSL(0, 0, 0);

			UpdateValues(GetCurrentColour());
		}

		// HSV V
		if (m_combo.GetCurSel() == 5)
		{
			SetHSV((unsigned short)GetDlgItemInt(IDC_HUE),
				(unsigned short)GetDlgItemInt(IDC_SATURATION),
				(unsigned short)((255 - y) / 255.0*100.0));
			SetHSL(0, 0, 0);

			UpdateValues(GetCurrentColour());
		}
	}

	// Alpha
	if (m_bAlpha)
	{
		int y = (p.y - rectA.top);
		CLAMP(y, 0, 255);

		SetAlpha((unsigned short)((float)(255 - y) / 255.0f*255.0f));
		UpdateValues(GetCurrentColour());
		//SendMessage(WM_PAINT, 0, 0);
	}
	SendMessage(WM_PAINT, 0, 0);
}

BOOL CMainDlg::OnSetCursor(CWindow wnd, UINT nHitTest, UINT message)
{
	SetCursor(m_arrow);
	return TRUE;
}

void CMainDlg::OnDropDown(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	::SendMessage(m_combomenu.m_hWnd, CB_SETDROPPEDWIDTH, 70, 0);
}






