#pragma once

#include "constParameter.h"
#include "ofxAnimatableOfColor.h"
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

class symbolAnim
{
public:
	symbolAnim();
	void update(float delta);
	void setSymbol(symbol* ptr);
	bool toSymbol(symbol& target, float duration);

private:
	array<ofxAnimatableOfColor, cMetaAiSymbolNodeNum> _animList;
	symbol* _animSymbol;

public:
	bool _startAnim;
	ofEvent<void> _onSymbolAnimFinish;
};