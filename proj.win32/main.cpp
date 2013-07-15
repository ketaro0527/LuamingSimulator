#include "main.h"
#include "AppDelegate.h"
#include "CCEGLView.h"

USING_NS_CC;

std::string wcs_to_mbs(std::wstring const& str, std::locale const& loc = std::locale())
{
    typedef std::codecvt<wchar_t, char, std::mbstate_t> codecvt_t;
    codecvt_t const& codecvt = std::use_facet<codecvt_t>(loc);
    std::mbstate_t state = 0;
    std::vector<char> buf((str.size() + 1) * codecvt.max_length());
    wchar_t const* in_next = str.c_str();
    char* out_next = &buf[0];
    codecvt_t::result r = codecvt.out(state, 
        str.c_str(), str.c_str() + str.size(), in_next, 
        &buf[0], &buf[0] + buf.size(), out_next);
    return std::string(&buf[0]);
}

// uncomment below line, open debug console
#define USE_WIN32_CONSOLE

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

#ifdef USE_WIN32_CONSOLE
    AllocConsole();
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
#endif
	int ret = 0;
	if (lpCmdLine != NULL && lstrlen(lpCmdLine) != 0)
	{
		std::locale::global(std::locale(""));
		std::wstring wpath = lpCmdLine;
		std::string path = wcs_to_mbs(wpath);
		
		CCFileUtils::luaming_project_path = path;
		path.append("\\assets");
		CCFileUtils::luaming_assets_path = path;

		// create the application instance
		AppDelegate app;
		CCEGLView* eglView = CCEGLView::sharedOpenGLView();
		eglView->setFrameSize(480, 800);

		ret = CCApplication::sharedApplication()->run();
	}
	else
	{
		printf("ERROR: Please enter Luaming Project Path!!\n");
		getchar();
	}
	
#ifdef USE_WIN32_CONSOLE
	FreeConsole();
#endif

    return ret;
}
