#pragma once

#include "constParameter.h"

struct blobData
{
	blobData()
		:x(0)
		, y(0)
		, width(0)
		, height(0)
	{}

	uint16_t x, y;
	uint16_t width, height;
};

struct kinectPackage
{
	kinectPackage()
		:_kid(0)
	{}
	uint8_t _kid;
	blobData _blobData[cKMaxBlobNumEach];
};
static const int cKinectUDPLength = sizeof(kinectPackage);

struct trackBlob
{
	trackBlob()
		:_bid(-1)
	{}
	trackBlob(blobData blob)
		:_bid(-1)
	{
		_rect.set(blob.x, blob.y, blob.width, blob.height);
	}

	int _bid;
	ofRectangle _rect;
};

struct blobDistSet
{
	blobDistSet(int id, float dist)
		:_id(id)
		,_dist(dist)
	{}
	int _id;
	float _dist;
};