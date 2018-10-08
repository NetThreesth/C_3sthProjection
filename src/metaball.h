#pragma once

#include "constParameter.h"

class metaNode
{
public:
	metaNode() 
		:_useBasic(false)
		, _display(false)
	{};
	void setup(int x, int y, bool useBasicFlow);
	void update(float delta);
	void draw();
	ofVec3f getPosAndR();
	bool isDisplay();
private:
	bool _display, _useBasic;
	ofVec2f _vec, _pos, _acc;
	float _r;
};

class metaball
{
public:
	metaball() {};
	void setup();
	void update(float delta);
	void draw();
	void drawNode();

private:
	void nodeUpdate(float delta);

private:
	array<metaNode, cMetaballNum> _nodeSet;
	ofImage _tmp;
	ofFbo _canvas;
	ofShader _meta;


};