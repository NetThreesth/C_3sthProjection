#include "viewCam.h"

//------------------------------------
viewCam::viewCam()
	:_eState(eViewArms)
	,_pos(cViewCamDefaultPos)
	,_target(cViewCamDefaultTarget)
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

	stateCheck(delta);
	
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
	_isStart = false;
}

//------------------------------------
void viewCam::start()
{
	_isStart = true;
	_eState = eViewArms;
	_timer = cViewArmsT;
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
void viewCam::stateCheck(float delta)
{

	switch (_eState)
	{
	case eViewArms:
	{
		_timer -= delta;
		if (_timer <= 0.0f)
		{
			_animZ.reset(_pos.z);
			_animZ.setDuration(6.0);
			_animZ.setCurve(AnimCurve::QUADRATIC_EASE_OUT);
			_animZ.animateTo(0.0f);

			_animY.reset(_pos.y);
			_animY.setDuration(8.0);
			_animY.animateTo(500.0f);
			_eState = eArmsToThreeBody;
		}
		break;
	}
	case eArmsToThreeBody:
	{
		if (_animY.hasFinishedAnimating() && _animY.getPercentDone() == 1.0f)
		{
			_animY.setDuration(10.0);
			_animY.animateTo(4500.0f);
			_eState = eViewThreeBody;
		}
		else
		{
			animToThreeBody();
		}
		break;
	}
	case eViewThreeBody:
	{
		if (_animY.hasFinishedAnimating() && _animY.getPercentDone() == 1.0f)
		{
			_eState = eViewSymbol;
		}
		else
		{
			_pos.y = _animY.getCurrentValue();
		}
		break;
	}
	case eViewSymbol:
	{
		break;
	}
	}
}


//------------------------------------
void viewCam::animToThreeBody()
{
	ofVec3f newPos = _pos;
	newPos.y = _animY.getCurrentValue();
	newPos.z = _animZ.getCurrentValue();
	_target = (newPos - _pos).normalized();
	_pos = newPos;
}
