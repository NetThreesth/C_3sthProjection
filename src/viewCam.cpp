#include "viewCam.h"

//------------------------------------
viewCam::viewCam()
	:_eState(eViewWait)
	,_pos(cViewCamDefaultPos)
	,_target(cViewCamDefaultTarget)
	, _isStart(false)
{
	_canvas.allocate(cViewCamSize.width, cViewCamSize.height, GL_RGB);
	_cam.disableMouseInput();

}

//------------------------------------
void viewCam::update(float delta)
{
	updateCam();
	if (!_isStart)
	{
		return;
	}

	_animY.update(delta);
	_animZ.update(delta);
	
	stateCheck(delta);
	
}

//------------------------------------
void viewCam::drawCamera()
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
	_pos = cViewCamDefaultPos;
	_target = cViewCamDefaultTarget;

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
			_animY.animateTo(cAmrsCeilTopPos.y);
			_eState = eArmsToThreeBody;
			ofNotifyEvent(_onViewStateChange, _eState, this);
		}
		break;
	}
	case eArmsToThreeBody:
	{
		if (_animY.hasFinishedAnimating() && _animY.getPercentDone() == 1.0f)
		{
			_animY.setDuration(10.0);
			_animY.animateTo(cSymbolViewHeight.y);
			_eState = eViewThreeBody;
			ofNotifyEvent(_onViewStateChange, _eState, this);
		}
		else
		{
			animToThreeBody();
		}
		break;
	}
	case eViewThreeBody:
	case eViewThreeBodyAndSymbol:
	{
		if (_animY.hasFinishedAnimating() && _animY.getPercentDone() == 1.0f)
		{
			_eState = eViewSymbol;
			ofNotifyEvent(_onViewStateChange, _eState, this);
			_timer = cSymbolDisplayTime;
		}
		else
		{
			_pos.y = _animY.getCurrentValue();

			if (_eState == eViewThreeBody && _pos.y > cSymbolDisplayHeight)
			{
				_eState = eViewThreeBodyAndSymbol;
				ofNotifyEvent(_onViewStateChange, _eState, this);
			}
		}
		break;
	}
	case eViewSymbol:
	{
		_timer -= delta;
		if (_timer <= 0.0f)
		{
			_eState = eSymbolToWait;
			_timer = cViewFadeT;
			ofNotifyEvent(_onViewStateChange, _eState, this);
		}
		break;
	}
	case eSymbolToWait:
	{
		_timer -= delta;
		if (_timer <= 0.0f)
		{
			_eState = eViewWait;
			ofNotifyEvent(_onViewStateChange, _eState, this);
		}
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

#pragma region Camera
//------------------------------------
void viewCam::draw()
{
	draw(0, 0, cViewCamSize.width, cViewCamSize.height);
}

//------------------------------------
void viewCam::draw(int x, int y, int width, int height)
{
	ofPushStyle();
	ofSetColor(255);
	_canvas.draw(x, y, width, height);
	ofPopStyle();
}

//------------------------------------
void viewCam::updateCam()
{
	_cam.setPosition(_pos);
	_cam.lookAt(_pos + _target);
}

//------------------------------------
void viewCam::begin()
{
	_canvas.begin();
	ofClear(50);
	_cam.begin();

}

//------------------------------------
void viewCam::end()
{
	_cam.end();
	_canvas.end();
}

//------------------------------------
ofNode & viewCam::getCam()
{
	return _cam;
}

#pragma endregion


