#pragma once

#include "kinectUnit.h"


class kinectMgr
{
private:
	void setup();
	void update(float delta);
	void draw();

private:
	void merge();

public:
	list<blobData> _megreBlobList;
	array<kinectUnit, cKinectNum> _kinectList;
};