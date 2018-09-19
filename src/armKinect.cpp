#include "armKinect.h"

//--------------------------
armKinect::armKinect()
	:_isStart(false)
	, _isReady(false)
	, _isLoad(false)
	, _haveNext(false)
	,_displayPtr(nullptr)
	,_bufferPtr(nullptr)
	, _totalFrameIdx(0)
	, _bufferFrameIdx(0)
	, _frameTimer(cFrameTime)
{
	for (int i = 0; i < cArmBufferSize; i++)
	{
		for (int j = 0; j < cArmKinectNum; j++)
		{
			_ping[i][j].clear();
			_ping[i][j].setMode(ofPrimitiveMode::OF_PRIMITIVE_POINTS);

			_pong[i][j].clear();
			_pong[i][j].setMode(ofPrimitiveMode::OF_PRIMITIVE_POINTS);
		}
	}

	for (int i = 0; i < cArmKinectNum; i++)
	{
		_kPos[i].set(0);
		_kRot[i].set(0);
	}
	_kPos[1].set(-136, 0, -424);
	_kRot[1].set(0, 223, 0);
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
		if (_totalFrameIdx == _startF)
		{
			//For Start
			checkBuffer();
		}
		else if (_totalFrameIdx == cArmEndFrame - 1)
		{
			//End
			_isStart = false; 
		}


		_totalFrameIdx = _startF + _bufferFrameIdx;
		_bufferFrameIdx = (_bufferFrameIdx + 1) % cArmBufferSize;
		_frameTimer = cFrameTime;

		if (_bufferFrameIdx == 0)
		{
			checkBuffer();
		}
		
	}
}

//--------------------------
void armKinect::draw()
{
	if (!_isStart || _displayPtr == nullptr)
	{
		return;
	}

	ofPushStyle();
	ofSetColor(255);
	for (int i = 0; i < cArmKinectNum; i++)
	{
		ofPushMatrix();
		ofTranslate(_kPos[i]);
		ofRotateX(_kRot[i].x);
		ofRotateY(_kRot[i].y);
		ofRotateZ(_kRot[i].z);

		(*_displayPtr)[_bufferFrameIdx][i].draw();

		ofPopMatrix();
	}
	ofPopStyle();
}

//--------------------------
void armKinect::start()
{	
	_haveNext = false;
	_bufferFrameIdx = 0;

	_displayPtr = &_ping;
	_bufferPtr = &_pong;
	
	//Load Buffer
	_startF = cArmStartFrame;
	_endF = cArmStartFrame + cArmBufferSize;
	loadFrame(_displayPtr, _startF, _endF);

	_totalFrameIdx = _startF;
}

//--------------------------
int armKinect::getFrame()
{
	return _totalFrameIdx;
}

//--------------------------
void armKinect::checkBuffer()
{
	if (_isLoad)
	{
		return;
	}
	
	if (_haveNext)
	{
		cout << "SWAP" << endl;
		swap(_displayPtr, _bufferPtr);
		_haveNext = false;
	}
	_startF = _endF;
	_endF = MIN((_startF + cArmBufferSize), cArmEndFrame);
	loadFrame(_bufferPtr, _startF, _endF);
	
}

//--------------------------
bool armKinect::loadFrame(armBuffer* loadPtr, int start, int end)
{
	if (_isLoad)
	{
		ofLog(OF_LOG_WARNING, "[armKinect::loadFrame]Still loading");
		return false;
	}
	_isLoad = true;

	thread loadT([&](armBuffer* ptr, int sF, int eF) {
		load(ptr, 0, sF, eF);
		load(ptr, 1, sF, eF);
		
		if (_isStart)
		{
			_haveNext = true;
		}
		else
		{
			_isStart = true;
		}
		
		_isLoad = false;
		
		cout << "Finish" << endl;
	}, loadPtr, start, end);
	loadT.detach();
	return true;
}

//--------------------------
void armKinect::load(armBuffer* loadPtr, int kinectIdx, int start, int end)
{
	int frameIdx = 0;
	for (int photoIdx = start; photoIdx < end; photoIdx++, frameIdx++)
	{
		(*loadPtr)[frameIdx][kinectIdx].clear();
		cout << photoIdx << endl;
		ofShortImage depth;
		ofImage color;
		depth.load("arm/kinect" + ofToString(kinectIdx) + "/depth_" + ofToString(photoIdx) + ".png");
		auto dPixel = depth.getPixels();

		for (int i = 0; i < (512 * 424); i += 2)
		{
			ofVec3f pos;
			pos.x = ofMap(dPixel[i * 3], 100, 65535, -400, 400);
			pos.y = ofMap(dPixel[i * 3 + 1], 100, 65535, -400, 400);
			pos.z = ofMap(dPixel[i * 3 + 2], 100, 65535, -50, -400);
			if (pos.x <= -400 && pos.y <= -400 && pos.z <= -49)
			{
				continue;
			}
			(*loadPtr)[frameIdx][kinectIdx].addVertex(pos);
		}
	}
}
