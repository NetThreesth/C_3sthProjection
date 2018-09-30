#pragma once

#include "constParameter.h"

typedef array<ofVboMesh, cArmKinectNum> armFrame;
typedef array<armFrame, cArmBufferSize> armBuffer;

class armKinect
{
public:
	armKinect();
	void update(float delta);
	void draw(ofVec3f shift);
	void reset();
	void play();
	int getFrame();
private:
	void checkBuffer();
	bool loadFrame(armBuffer* loadPtr, int start, int end);
	void load(armBuffer* loadPtr, int kinectIdx, int start, int end);

private:
	bool _isSetup, _isReady, _isPlay, _isLoad, _haveNext;
	float _frameTimer;

	armBuffer *_displayPtr, *_bufferPtr;
	armBuffer _ping, _pong;
	array<ofVec3f, cKinectNum> _kPos, _kRot;
	int _totalFrameIdx, _bufferFrameIdx;
	int _startF, _endF;
	 
};