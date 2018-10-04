#pragma once

#include "constParameter.h"

class particle
{
public:
	particle();
	void update(float delta, int maxY);
	ofVec3f getDrawPos();
	bool checkLife();

private:
	ofVec3f _pos;
	float _speed, _timer;
	float _radius, _radiusV;
	float _theta1, _theta2;
	bool _isDead;
};
