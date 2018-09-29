#pragma once

#include "constParameter.h"
#include "metaball.h"
#include "metaAiSymbol.h"
#include "flowField.h"
#include "ofxLayerMask.h"

class vSymbolMirror
{
public:
	vSymbolMirror()
		:_isSetup(false)
		,_isStart(false)
	{}
	void setup(int width, int height);
	void update(float delta);
	void draw(ofVec3f pos);

	void start();
	void stop();

private:
	void initLayerMask(int width, int height);

private:
	bool _isSetup, _isStart;
	ofxLayerMask _mirrorContext;


#pragma region Symbol & Metaball
private:
	void initSymbol();
	void drawOnMirror();

	vector<symbol> _symbolList;
	metaAiSymbolDisplay _symbolDisplay;

private:
	ofEasyCam _cam;
	int _symbolIndex;
	metaball _mb;
#pragma endregion


#pragma region Mirror & Mask
private:
	bool load(string name);
	bool loadMirror(string name);
	void drawMirror();
private:
	ofImage _mask;
	ofVboMesh _mirror;
#pragma endregion


};