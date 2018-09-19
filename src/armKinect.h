#pragma once

#include "constParameter.h"

typedef array<ofVboMesh, cArmKinectNum> armFrame;
typedef array<armFrame, cArmBufferSize> armBuffer;

class armKinect
{
public:
	armKinect();
	void setup();
	void update(float delta);
	void draw();
	void start();
	
private:
	void load(armBuffer* loadPtr, int kinectIdx, int start, int end);

private:
	bool _isStart;
	float _frameTimer;

	armBuffer *_displayPtr, *_bufferPtr;
	armBuffer _ping, _pong;
	array<ofVec3f, cKinectNum> _kPos, _kRot;
	int _frameIdx;
	 
};