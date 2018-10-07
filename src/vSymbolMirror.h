#pragma once

#include "constParameter.h"
#include "metaball.h"
#include "metaAiSymbol.h"
#include "flowField.h"
#include "ofxLayerMask.h"
#include "ofxAnimatableFloat.h"

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
private:
	void initSymbol();
	void updateToMirror();
	void drawSymbol(ofVec3f pos);

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
	void updateMirror(float delta);
	void resetMirror();
	void drawMirror(ofVec3f pos);
private:
	ofImage _mask;
	ofVboMesh _mirror;
#pragma endregion


};