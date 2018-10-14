#include "config.h"

//--------------------------
void config::init()
{
	initGUI();
	loadConfig();
	loadGuiSetting();
}

//--------------------------
void config::save()
{
	_gui.saveToFile("_guiConfig.xml");

}

//--------------------------
void config::draw()
{
	_fps = ofToString(ofGetFrameRate());
	_gui.draw();
}

//--------------------------
void config::initGUI()
{
	_gui.setup("", "_guiConfig.xml");
	_gui.add(_armsAlpha.setup("Arms Alpha", 255, 0, 255));
	_gui.add(_ceilingAlpha.setup("Ceiling Alpha", 50, 0, 255));
	_gui.add(_spaceAlpha.setup("Space Alpha", 50, 0, 255));
	_gui.add(_multiCamDist.setup("Multi-Camera dist", 100, 50, 500));
	_gui.add(_symbolMirrorAlpha.setup("Symbol Mirror Alpha", 200, 0, 255));
	_gui.add(_symbolRotateSpeed.setup("Symbol Rotate Speed(deg/s)", 10.0, 1.0, 60.0));
	_gui.add(_symbolChangeT.setup("Symbol Translate Duration", 5.0, 3.0, 30.0));
	_gui.add(_metaballSize.setup("Metaball Size", 1.0, 0.5, 2.0));
	_gui.add(_metaballFieldBasic.setup("Field Basic", 1.0, 0.0, 5.0));

	_gui.add(_threeBodyAlpha.setup("Three Body Alpha", 100, 0, 255));

	_gui.add(_faderT.setup("Cover fadeing Time", 3.0, 1.0, 20.0));
	_gui.add(_fps.setup("FPS", "0.0"));
	
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
	_exWindowHeight = xml.getValue("ScreenHeight", 1920);

	_exCamStartWaitT = xml.getValue("camStartWaitT", 20.0f);
	_exCamToCeilingT = xml.getValue("camToCeilingT", 10.0f);
	_exCamToSymbolT = xml.getValue("camToSymbolT", 10.0f);

	_exMetaballBasicNum = xml.getValue("metaballBasicNum", 20);
}

//--------------------------
void config::loadGuiSetting()
{
	_gui.loadFromFile("_guiConfig.xml");
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


