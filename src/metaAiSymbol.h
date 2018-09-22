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
	void setup(int size, int range, float thresholdMin, float thresholdMax);
	void update(float delta);
	void draw();
	void setSymbol(symbol& data);
	void toSymbol(symbol& toData);

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
	ofPixels _text;

	ofVboMesh _symbolLine, _symbolMesh;
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

#pragma region Light
private:
	void lightSetup();
	void lightUpdate();
	void lightEnable();
	void lightDisable();

private:

	//light
	ofLight _pointLight, _spotLight, _directionalLight;
#pragma endregion

};

