#include "main.h"
#include "AppDelegate.h"
#include "CCEGLView.h"

USING_NS_CC;

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
		std::wstring wpath = lpCmdLine;
		std::string path(wpath.begin(), wpath.end());
	
		CCFileUtils::luaming_project_path = path;
		path.append("\\assets");
		CCFileUtils::luaming_assets_path = path;

		// create the application instance
		AppDelegate app;
		CCEGLView* eglView = CCEGLView::sharedOpenGLView();
		eglView->setFrameSize(800, 480);

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
