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