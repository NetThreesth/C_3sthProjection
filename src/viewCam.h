#pragma once

#include "constParameter.h"
#include "viewState.h"
#include "ofxAnimatableFloat.h"

class viewCam
{
public:
	viewCam();
	void update(float delta);
	void drawCamera();
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

#pragma region Camera
public:
	void draw();
	void draw(int x, int y, int width, int height);
	void updateCam();
	void begin();
	void end();
	ofNode& getCam();
private:
	ofFbo _canvas;
	ofEasyCam _cam;
#pragma endregion
	
public:
	ofEvent<eViewState> _onViewStateChange;
};