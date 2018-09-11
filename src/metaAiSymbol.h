#pragma once

#include "constParameter.h"
#include "ofxAnimatableOfPoint.h"

class symbolNode {
public:
	symbolNode();
	void init(int x, int y);
	void update(float time, float range);
	void draw(float size);

	ofVec3f getPos();
private:
	ofVec3f _pos;
};

class symbol
{
public:
	void load(string path);
	
public:
	array<bool, cMetaAiSymbolNodeNum> _symbolFlag;
	array<ofColor, cMetaAiSymbolNodeNum> _symbolColor;
	ofImage _symbol;
};

class metaAiSymbolDisplay
{
public:
	metaAiSymbolDisplay();
	void setup(int size, int range, float threshold);
	void update(float delta);
	void draw();
	void setSymbol(symbol& data);
	void toSymbol(symbol& toData);

private:
	void drawNode();
	void rebuildLine(symbol* data);

	
	void initBaseDist(float unit);
	vector<int> getNearIndex(int x, int y, int maxNum);
	void index2xy(int index, int& x, int& y);

private:
	int _displaySize;
	float _moveRange, _threshold, _nodeSize;
	ofPixels _text;

	ofVboMesh _symbolLine;
	array<vector<int>, cMetaAiSymbolNodeNum> _symbolLineCheck;
	array<symbolNode, cMetaAiSymbolNodeNum> _symbolNode;
	array<float, cMetaAiSymbolSize * 2> _symbolBaseDist;
	symbol* _symbolRef, *_symbolTarget;
	float _mainTimer;

private:
	void checkState(float delta);
	void updateLine();
	void toCenter();
	void toTargetSymbol();
private:
	enum eAnimState
	{
		eAnimDisplay,
		eAnimOutput,
		eAnimIn,
	}_eState;
	vector<ofxAnimatableOfPoint> _animPointList;
	float _animTimer;
};

