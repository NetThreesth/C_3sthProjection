#pragma once

#include "constParameter.h"

class metaNode
{
public:
	metaNode() 
		:_useBasic(false)
	{};
	void setup(int x, int y, bool useBasicFlow);
	void update(float delta);
	void draw();
	ofVec3f getPosAndR();
private:
	bool _useBasic;
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

	void add(int num);
	void remove(int num);

private:
	void nodeUpdate(float delta);

private:
	list<metaNode> _nodeBasicSet, _nodeSet;
	ofImage _tmp;
	ofFbo _canvas;
	ofShader _meta;


};