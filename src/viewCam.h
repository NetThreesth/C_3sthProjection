#pragma once

#include "constParameter.h"
#include "viewState.h"
#include "ofxAnimatableFloat.h"

class viewCam
{
public:
	viewCam();
	void update(float delta);
	void draw();
	void reset();
	void start();

	ofVec3f getPos();
	ofVec3f getTarget();
private:
	void stateCheck();

private:
	bool _isStart;
	ofVec3f _pos, _target;
	eViewState _eState;

	ofxAnimatableFloat _animY, _animZ;
};