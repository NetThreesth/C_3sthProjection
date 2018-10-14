#pragma once

#include "baseSymbol.h"
#include "ofxAnimatableFloat.h"
#include "ofxAnimatableOfPoint.h"

class metaAiSymbolDisplay
{
public:
	metaAiSymbolDisplay();
	void setup(int size, int range, float thresholdMin, float thresholdMax);
	void update(float delta);
	void draw();
	void setSymbol(symbol& data);
	bool toSymbol(symbol& toData, float duration);
	
	void onRotateVChange(float& v);
private:
	void drawNode();
	void rebuildLine(symbol* data);
	void rebuildMesh(symbol* data);

	vector<int> getNearIndex(symbol * data, int index);

	
	void initBaseDist(float unit);
	vector<int> getPossibleIndex(int x, int y, int minNum, int maxNum);
	void index2xy(int index, int& x, int& y);

private:
	int _displaySize;
	float _moveRange, _tMin, _tMax, _nodeSize;
	float _rDeg, _rV;
	ofPixels _text;

	ofVboMesh _symbolLine, _symbolMesh;
	array<vector<int>, cMetaAiSymbolNodeNum> _symbolLineCheck;
	array<symbolNode, cMetaAiSymbolNodeNum> _symbolNode;
	array<float, cMetaAiSymbolSize * 2> _symbolBaseDist;
	symbol* _symbolRef, *_symbolTarget;
	float _mainTimer;

private:
	void checkState(float delta);
private:
	enum eAnimState
	{
		eAnimDisplay = 0,
		eAnimTranslate
	}_eState;
	symbolAnim _animSymbol;

};

