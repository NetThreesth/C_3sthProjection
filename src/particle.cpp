#include "particle.h"

//--------------------------------------
particle::particle()
	:_theta1(ofRandom(0, 360))
	, _theta2(ofRandom(0, 360))
	, _speed(ofRandom(60, 180))
	, _radius(cPBaseRadius)
	, _radiusV(ofRandom(-30, -60))
	, _timer(0.0f)
	, _isDead(false)
{
	_pos.x = cos(_theta1)*_radius;
	_pos.y = 0;
	_pos.z = sin(_theta2)*_radius;
}

//--------------------------------------
void particle::update(float delta, int maxY)
{
	_timer += delta;
	_theta1 += delta * 0.01; //ofNoise modify factor
	_theta2 += delta * 0.01; //ofNoise modify factor
	_radius += _radiusV * delta; // move towwards center
	
	if (abs(_radius) > 400)
	{
		_radiusV *= -1;
	}

	_pos.x = cos(ofNoise(_theta1) * 360)*_radius;
	_pos.y = _speed*ofNoise(_theta1 + _theta2) * 2 * _timer;
	_pos.z = sin(ofNoise(_theta2) * 360)*_radius;
	

	//if the particle higher than space.y
	if (_pos.y > maxY) {
		_isDead = true;
	}
}

//--------------------------------------
ofVec3f particle::getDrawPos()
{
	return _pos;
}

//--------------------------------------
bool particle::checkLife()
{
	return _isDead;
}
