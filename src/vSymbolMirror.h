#pragma once

#include "constParameter.h"
#include "metaball.h"
#include "metaAiSymbol.h"
#include "flowField.h"
#include "serverReq.h"
#include "ofxLayerMask.h"
#include "ofxAnimatableFloat.h"
#include "Poco/Base64Decoder.h"

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

	void debugUpdate(float delta);
	void debugDraw();

	void reset();
	void start();
	void stop();

	void displayMirror();
	void displayContent();

private:
	void initLayerMask(int width, int height);

private:
	bool _isSetup, _isStart;
	ofxAnimatableFloat _animMirrorAlpha, _animSymbolAlpha;
	ofxLayerMask _mirrorContext;


#pragma region Symbol & Metaball
public:
	void addMetaball(int num);
	void removeMetaball(int num);
	void onGetPattern(string& symbol64);
private:
	bool initSymbol();
	void loadSymbol();
	void updateSymbol(float delta);
	void updateToMirror();
	void drawSymbol(ofVec3f pos);
	void resetSymbol();

	vector<symbol> _symbolList;
	metaAiSymbolDisplay _symbolDisplay;

private:
	int _symbolIndex;
	float _translateTimer;
	metaball _mb;
	
#pragma endregion

#pragma region Mirror & Mask
private:
	bool load(string name);
	bool loadMirror(string name);
	void updateMirror(float delta);
	void resetMirror();
	void drawMirror(ofVec3f pos);
private:
	ofImage _mask;
	ofVboMesh _mirror;
#pragma endregion


};