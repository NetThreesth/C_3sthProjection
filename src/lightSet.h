#pragma once

#include "constParameter.h"

class lightSet
{
public:
	void setup();
	void update(float delta);
	void begin();
	void end();
private:
	//light
	ofLight _pointLight, _spotLight, _directionalLight;
};