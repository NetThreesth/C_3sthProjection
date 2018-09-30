#include "viewCam.h"

//------------------------------------
viewCam::viewCam()
	:_eState(eViewArms)
	,_pos(0, 0, 400)
	,_target(0, 0, -1)
{
}

//------------------------------------
void viewCam::update(float delta)
{
	if (!_isStart)
	{
		return;
	}

	_animY.update(delta);
	_animZ.update(delta);

	ofVec3f newPos = _pos;
	newPos.y = _animY.getCurrentValue();
	newPos.z = _animZ.getCurrentValue();
	_target = (newPos - _pos).normalized();
	_pos = newPos;
}

//------------------------------------
void viewCam::draw()
{
	ofPushStyle();
	ofSetColor(255);
	ofDrawSphere(_pos, 10);

	ofSetColor(255, 0, 0);
	ofDrawLine(_pos, _pos + _target * 30);
	ofPopStyle();
}

//------------------------------------
void viewCam::reset()
{
}

//------------------------------------
void viewCam::start()
{
	_animZ.reset(0);
	_animZ.setDuration(6.0);
	_animZ.setCurve(AnimCurve::QUADRATIC_EASE_OUT);
	_animZ.animateFromTo(400.0f, 0.0f);

	_animY.reset(0);
	_animY.setDuration(8.0);
	_animY.animateFromTo(0.0f, 500);
	_isStart = true;
}

//------------------------------------
ofVec3f viewCam::getPos()
{
	return _pos;
}

//------------------------------------
ofVec3f viewCam::getTarget()
{
	return _target;
}

//------------------------------------
void viewCam::stateCheck()
{
}
