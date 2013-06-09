#include "cocos2d.h"
#include "CCEGLView.h"
#include "AppDelegate.h"
#include "CCLuaEngine.h"
#include "SimpleAudioEngine.h"
#include "cJSON.h"

using namespace CocosDenshion;

USING_NS_CC;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
    SimpleAudioEngine::end();
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());

    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // register lua engine
    CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
    CCScriptEngineManager::sharedManager()->setScriptEngine(pEngine);

	// get main script
	const std::string projectPath = CCFileUtils::luaming_project_path;
	const std::string assetsPath = CCFileUtils::luaming_assets_path;

	unsigned long fileSize = 0;
	cJSON* pJson = cJSON_Parse((const char*)CCFileUtils::sharedFileUtils()->getFileData(
		std::string(assetsPath + "\\LuamingProject.json").c_str(), "r", &fileSize));
	
	if (pJson == NULL)
		return false;

	cJSON* pProjectName = cJSON_GetObjectItem(pJson, "PROJECT_NAME");
	if (pProjectName != NULL)
	{
		int length = strlen(pProjectName->valuestring);
		WCHAR* title = new WCHAR[length+1];
		title[length] = NULL;
		MultiByteToWideChar(0, 0, pProjectName->valuestring, length, title, length);
		HWND handle = CCEGLView::sharedOpenGLView()->getHWnd();
		SetWindowText(handle, title);
	}

	cJSON* pPackageName = cJSON_GetObjectItem(pJson, "PACKAGE_NAME");
	if (pPackageName != NULL)
		CCLog("Package: %s", pPackageName->valuestring);

	std::string mainScriptPath = assetsPath;
	mainScriptPath.append("\\");
	cJSON* pMainScript = cJSON_GetObjectItem(pJson, "MAIN_SCRIPT");
	if (pMainScript == NULL)
		return false;
	mainScriptPath.append(pMainScript->valuestring);

	mainScriptPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(mainScriptPath.c_str());
    pEngine->executeScriptFile(mainScriptPath.c_str());

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();

    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();

    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
