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
	void stateCheck(float delta);

	void animToThreeBody();

private:
	bool _isStart;
	float _timer;
	ofVec3f _pos, _target;
	eViewState _eState;

	ofxAnimatableFloat _animY, _animZ;

	ofEvent<eViewState> _onViewStateChange;
};