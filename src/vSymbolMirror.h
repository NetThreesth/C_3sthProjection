#pragma once

#include "constParameter.h"
#include "metaball.h"
#include "metaAiSymbol.h"
#include "flowField.h"

class vSymbolMirror
{
public:
	vSymbolMirror()
		:_isSetup(false)
		,_isStart(false)
	{}
	void setup(int width, int height);
	void update(float delta);
	void draw();

	void start();
	void stop();
private:
	void initSymbol();
	void drawOnCanvas();
	
	vector<symbol> _symbolList;
	metaAiSymbolDisplay _symbolDisplay;

private:
	bool _isSetup, _isStart;
	ofEasyCam _cam;
	ofFbo _canvas;
	int _symbolIndex;
	metaball _mb;

};