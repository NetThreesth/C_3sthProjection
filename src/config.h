#pragma once

#include "constParameter.h"
#include "ofxXmlSettings.h"
#include "ofxGui.h"

class config
{
public:
	void init();
	void save();
	void draw();

private:
	void loadConfig();
	void loadGuiSetting();
	void saveGuiSetting();

public:
	bool _exOnlyMultiCam;
	bool _exIsAutoLoop;
	int _exWindowWidth, _exWindowHeight;



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