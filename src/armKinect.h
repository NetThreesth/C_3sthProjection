#pragma once

#include "constParameter.h"

typedef array<ofVboMesh, cArmKinectNum> armFrame;
typedef array<armFrame, cArmBufferSize> armBuffer;

class armKinect
{
public:
	armKinect();
	void update(float delta);
	void draw();
	void start();
	int getFrame();
private:
	void checkBuffer();
	bool loadFrame(armBuffer* loadPtr, int start, int end);
	void load(armBuffer* loadPtr, int kinectIdx, int start, int end);

private:
	bool _isStart, _isReady, _isLoad, _haveNext;
	float _frameTimer;

	armBuffer *_displayPtr, *_bufferPtr;
	armBuffer _ping, _pong;
	array<ofVec3f, cKinectNum> _kPos, _kRot;
	int _totalFrameIdx, _bufferFrameIdx;
	int _startF, _endF;
	 
};