#pragma once

#include "kinectBase.h"
#include "ofxNetwork.h"

class kinectUnit
{
public:
	kinectUnit()
		:_kinectId(-1)
	{}
	void setup(int id, int x, int y, int width, int height, int port);

	bool update(float delta);
	
	//Debug
	void draw();
	
public:
	ofRectangle	_range;
	blobData _blobList[cKMaxBlobNumEach];

private:
	float _timer;
	int _kinectId;
	ofxUDPManager _udp;
};