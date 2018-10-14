#pragma once

#include "constParameter.h"
#include "threeBody.h"

class vThreeBody
{
public:
	vThreeBody()
		:_isSetup(false)
		, _isStart(false)
	{}
	void setup(int skipStep);
	void update(float delta);
	void draw(ofVec3f pos);

	void start();
	void stop();

private:
	bool _isSetup, _isStart;

private:
	threeBody _tBody;

};