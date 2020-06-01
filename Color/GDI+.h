#include <gdiplus.h>
//#include <gdipluspath.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

class GdiPlusIniter
{
public:
	GdiPlusIniter()
	{
		Gdiplus::GdiplusStartupInput StartupInput;
		GdiplusStartup(&m_gdiplusToken, &StartupInput, NULL);
	}
	~GdiPlusIniter()
	{
		Gdiplus::GdiplusShutdown(m_gdiplusToken);
	}
private:
	ULONG_PTR m_gdiplusToken;
};