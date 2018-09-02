#pragma once

#include "ofMain.h"

static const int cMetaballNum = 10;

class metaNode
{
public:
	metaNode() {};
	void setup(int x, int y);
	void update(float delta);
	void draw();
	ofVec3f getData();
private:
	ofVec2f _vec, _pos;
	float _r;
};

class ofApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
		
private:
	void drawOnCanvas();

private:
	float _timer;
	array<metaNode, cMetaballNum> _nodeSet;
	ofImage _bg;
	ofFbo _canvas;
	ofImage _tmp;
	ofShader _meta;
};

