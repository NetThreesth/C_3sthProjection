#include "vThreeBody.h"

//--------------------------------------------------------------
void vThreeBody::setup()
{
	_tBody.setup();
	_isSetup = true;
}

//--------------------------------------------------------------
void vThreeBody::update(float delta)
{
	if (!_isSetup || !_isStart)
	{
		return;
	}

	_tBody.update(delta);
}

//--------------------------------------------------------------
void vThreeBody::draw(ofVec3f pos)
{
	if (!_isSetup || !_isStart)
	{
		return;
	}
	ofPushMatrix();
	ofTranslate(pos);
	ofRotateX(90);
	{
		_tBody.draw();
	}
	ofPopMatrix();
}

//--------------------------------------------------------------
void vThreeBody::start()
{
	_isStart = true;
	
}

//--------------------------------------------------------------
void vThreeBody::stop()
{
	_isStart = false;
	_tBody.reset();
	
}
