#include "config.h"

//--------------------------
void config::init()
{
	loadConfig();
	loadGuiSetting();
}

//--------------------------
void config::save()
{
}

//--------------------------
void config::draw()
{
}

//--------------------------
void config::loadConfig()
{
	ofxXmlSettings xml;
	if (!xml.load("_config.xml"))
	{
		ofLog(OF_LOG_WARNING, "[config::loadConfig]load config xml failed");
	}

	_exOnlyMultiCam = xml.getValue("OnlyMultiCam", false);
	_exIsAutoLoop = xml.getValue("isAutoLoop", false);
	_exWindowWidth = xml.getValue("FullScreenWidth", 1920);

	float aspect = xml.getValue("CamAspect", 1.78f);
	_exWindowHeight = xml.getValue("ScreenHeight", 1920);

}

//--------------------------
void config::loadGuiSetting()
{
}

//--------------------------
void config::saveGuiSetting()
{
}

#pragma region Singleton
config* config::pInstance = 0;
config * config::getInstance()
{
	if (pInstance == 0)
	{
		pInstance = new config();
	}
	return pInstance;
}

void config::destroy()
{
	if (pInstance != 0)
	{
		delete pInstance;
	}
}
#pragma endregion


