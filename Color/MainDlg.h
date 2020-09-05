// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "BitmapBtn.h"
#include "GroupBoxEx.h"
#include "EditEx.h"
#include "ListBoxEx.h"
#include "ComboBoxFx.h"
#include "GDI+.h"
#include "CGdiPlusBitmap.h"
#include "DropPerWnd.h"
#include "SelColWnd.h"
#include "PicColWnd.h"
#include "AboutDlg.h"
#include <windows.h>
#include "GTDrawHelper.h"
#include "ComboMenu.h"
#include <string>
#include <sstream>

// dialog color
#define COLOR_DLG_BK1		RGB(47, 49, 54)
#define COLOR_DLG_BK2		RGB(32, 34, 37)
#define COLOR_DLG_TEXT		RGB(174, 175, 178)
#define COLOR_SPACE_BORDER	RGB(174, 175, 178)
#define COLOR_EDIT_BORDER	RGB(54, 57, 62)
#define COLOR_EDIT_TEXT		RGB(219, 219, 219)


#define _IS_IN(min, max, x)  (((x)>(min)) && ((x)<(max)))
#define CLAMP(x, m, M){\
if ((x)<(m)) (x) = (m); \
if ((x)>(M)) (x) = (M); \
}

#define MAX(a, b, c) ((a)>(b)? ((a)>(c)?(a):(c)) : ((b)>(c)?(b):(c)))
#define MIN(a, b, c) ((a)<(b)? ((a)<(c)?(a):(c)) : ((b)<(c)?(b):(c)))


struct SColour
{
	unsigned short r, g, b;		// R G B
	unsigned short h, s, v;		// H S V
	unsigned short h1, s1, l;   // H S L
	unsigned short a;			// Alpha

	// 颜色空间转换
	void UpdateRGB();			// Updates RGB from HSV
	void UpdateHSV();			// Updates HSV from RGB
	void UpdateHSL();           // Updates HSL from RGB
	void RGB_F_HSL(int h, int s, int l);				// Updates RGB from HSL
	double HSL2RGB(double v1, double v2, double vH);	// HSL helper function
	void RGB_F_HTML(CString szValue);					// Updates RGB from HTML
	void RGB_F_ASS(CString szValue);					// updates RGB from ASS
};



class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
	public CMessageFilter, public CIdleHandler
{
public:
	enum { IDD = IDD_MAINDLG };

	CMainDlg();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_NCHITTEST, OnNcHitTest)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorStatic)
		MESSAGE_HANDLER(WM_UPDATECOLOR, OnUpdateColor) // 更新颜色
		COMMAND_CODE_HANDLER_EX(CBN_SELCHANGE, OnComboBoxSelectChange)
		COMMAND_HANDLER_EX(IDC_COMMENU, CBN_DROPDOWN, OnDropDown)
		MSG_WM_MOUSEMOVE(OnMouseMove)
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MSG_WM_LBUTTONDBLCLK(OnLButtonDblClk)
		MSG_WM_LBUTTONUP(OnLButtonUp)
		MSG_WM_MOVE(OnMove)
		MSG_WM_TIMER(OnTimer)
		MSG_WM_SETCURSOR(OnSetCursor)
		MSG_WM_CTLCOLOREDIT(OnCtlColorEdit)
		COMMAND_CODE_HANDLER_EX(EN_CHANGE, OnEditControlChange)
		COMMAND_HANDLER(IDC_CLOSE, BN_CLICKED, OnBnClickedClose)
		COMMAND_HANDLER(IDC_MIN, BN_CLICKED, OnBnClickedMin)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_ID_HANDLER(IDC_MENU, OnAbout)
		MSG_WM_SIZE(OnSize)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnAbout(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedClose(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedMin(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCtlColorStatic(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUpdateColor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	HBRUSH OnCtlColorEdit(CDCHandle dc, CEdit edit);
	void CloseDialog(int nVal);
	void OnMouseMove(UINT nFlags, CPoint point);
	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnLButtonUp(UINT nFlags, CPoint point);
	void OnMove(CPoint ptPos);
	void OnComboBoxSelectChange(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnLButtonDblClk(UINT nFlags, CPoint point);
	void OnTimer(UINT_PTR nIDEvent);
	void OnEditControlChange(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnDropDown(UINT uNotifyCode, int nID, CWindow wndCtl);
	BOOL OnSetCursor(CWindow wnd, UINT nHitTest, UINT message);
	void OnSize(UINT nType, CSize size);


public:
	int m_ncHeight = 28;  // 标题栏高度

	// 控件自绘
	CBitmapBtn m_close, m_cancel, m_ok, m_min;
	CGroupBoxEx m_grColor, m_grRgb, m_grHsl, m_grHsv;
	CEditEx m_eRed, m_eBlue, m_eGreen;
	CEditEx m_eAss, m_eHtml, m_eAlpha;
	CEditEx m_eHue, m_eSaturation, m_eLightness;
	CEditEx m_eHue2, m_eSaturation2, m_eValue;
	CComboBoxFx m_combo;
	CComboMenu m_combomenu;
	CAboutDlg m_dAboutDlg;
	CStatic m_prompt;

public:
	void SetRGB(unsigned short r, unsigned short g, unsigned short b);
	void SetHSV(unsigned short h, unsigned short s, unsigned short v);
	void SetHSL(unsigned short h, unsigned short s, unsigned short l);
	void SetRGB_F_HSL(unsigned short h, unsigned short s, unsigned short l);
	void SetRGB_F_HTML();
	void SetRGB_F_ASS();
	void SetAlpha(unsigned short a);

	// 获取结构体数值
	SColour GetCurrentColour();

	GdiPlusIniter gdi;      // 初始化 GDI
	void SetSel2(int nID);  // 处理文本框光标位置
	void ColorFromPos();	
	void UpdateValues(struct SColour col);

	BOOL m_bColor; // mouse in color1
	BOOL m_bHue;   // mouse in color2
	BOOL m_bAlpha; // mouse in alpha
	CGdiPlusBitmapResource m_DropPng;   // 加载png
	CDropDlg m_drop;
	CSelColDlg m_selcol;
	CPicColDlg m_piccol;
	CStatic m_dper;
	HCURSOR m_cDrop;

private:

	SColour CurrCol;

public:

//填充数据

	BITMAPINFO bmp_info;
	HBITMAP bmp_handle;
	DWORD *bmp_data;
	
	// Color1
	void DoDrawRGB();
	void DoCalculateColors(COLORREF &lefttop,
		COLORREF &righttop, COLORREF &leftbottom, COLORREF &rightbottom);
	void DoDrawHSV_Hue();
	void DoDrawHSV_Sat();
	void DoDrawHSV_Val();


	// Color2
	BITMAPINFO bmp_info2;
	HBITMAP bmp_handle2;
	DWORD *bmp_data2;
	DWORD *row_buffer;
	int m_nBuffSize;
	void OnDrawSilder();
	void DrawRGB_ALL();
	void DrawHSV_H();
	void DrawHSV_S();
	void DrawHSV_V();
	void DrawAlpha();

	// Alpha
	BITMAPINFO bmp_info3;
	HBITMAP bmp_handle3;
	DWORD *bmp_data3;

public:
	void SetClipboardText(CString str);
	BOOL m_bText;  // 显示提示信息
	HCURSOR m_arrow;
};
