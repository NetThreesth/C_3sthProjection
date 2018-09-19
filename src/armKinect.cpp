#include "armKinect.h"

//--------------------------
armKinect::armKinect()
	:_isStart(false)
	,_displayPtr(nullptr)
	,_bufferPtr(nullptr)
	, _frameIdx(0)
	, _frameTimer(cFrameTime)
{
	for (int i = 0; i < cArmBufferSize; i++)
	{
		for (int j = 0; j < cKinectNum; j++)
		{
			_ping[i][j].clear();
			_ping[i][j].setMode(ofPrimitiveMode::OF_PRIMITIVE_POINTS);

			_pong[i][j].clear();
			_pong[i][j].setMode(ofPrimitiveMode::OF_PRIMITIVE_POINTS);
		}
	}

	for (int i = 0; i < cKinectNum; i++)
	{
		_kPos[i].set(0);
		_kRot[i].set(0);
	}
}

//--------------------------
void armKinect::setup()
{
	_displayPtr = &_ping;
	_bufferPtr = &_pong;

	//Load Buffer
	load(_displayPtr, 0, cArmStartFrame, cArmStartFrame + cArmBufferSize);
	load(_displayPtr, 1, cArmStartFrame, cArmStartFrame + cArmBufferSize);
}

//--------------------------
void armKinect::update(float delta)
{
	if (!_isStart || _displayPtr == nullptr)
	{
		return;
	}

	_frameTimer -= delta;
	if (_frameTimer <= 0.0f)
	{
		_frameIdx = (_frameIdx + 1) % cArmFrameNum;
		_frameTimer = cFrameTime;
	}
}

//--------------------------
void armKinect::draw()
{
	if (!_isStart || _displayPtr == nullptr)
	{
		return;
	}

	for (int i = 0; i < cKinectNum; i++)
	{
		ofPushMatrix();
		ofTranslate(_kPos[i]);
		ofRotateX(_kRot[i].x);
		ofRotateY(_kRot[i].y);
		ofRotateZ(_kRot[i].z);

		auto a = (*_displayPtr)[_frameIdx][i];

		ofPopMatrix();
	}
}

//--------------------------
void armKinect::start()
{
	_isStart = 0;
	_frameIdx = 0;
	_
}

//--------------------------
void armKinect::load(armBuffer* loadPtr, int kinectIdx, int start, int end)
{
}
