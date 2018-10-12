#pragma once

#include "ofxXmlSettings.h"
#include "ofxGui.h"

class config
{
public:
	void init();
	void save();
	void draw();

private:
	void initGUI();
	void loadConfig();
	void loadGuiSetting();
	void saveGuiSetting();

public:

	//Config
	bool _exOnlyMultiCam;
	bool _exIsAutoLoop;
	int _exWindowWidth, _exWindowHeight;
	float _exCamStartWaitT, _exCamToCeilingT, _exCamToSymbolT;
	int _exMetaballBasicNum;

	//Gui
	ofxPanel _gui;
	ofxIntSlider _armsAlpha, _ceilingAlpha, _spaceAlpha;


//-------------------
//Singleton
//-------------------
private:
	config()
	{
	};
	~config()
	{
		config::destroy();
	}
	config(config const&);
	void operator=(config const&);

public:
	static config* getInstance();
	static void destroy();

private:
	static config *pInstance;
};