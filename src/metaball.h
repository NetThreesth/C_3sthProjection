#pragma once

#include "constParameter.h"

class metaNode
{
public:
	metaNode() {};
	void setup(int x, int y, ofColor c);
	void update(float delta);
	void draw();
	ofVec3f getPosAndR();
	ofColor getColor();
private:
	ofColor _color;
	ofVec2f _vec, _pos, _acc;
	float _r;
};

class metaball
{
public:
	metaball() {};
	void setup(int width, int height);
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