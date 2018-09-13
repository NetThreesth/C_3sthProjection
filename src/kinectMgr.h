#pragma once

#include "kinectUnit.h"


class kinectMgr
{
public:
	void setup();
	void update(float delta);
	void draw();

private:
	void checkBlob();
	void checkMerge(blobData& blob);

public:
	list<blobData> _mergeBlobList;
	array<kinectUnit, cKinectNum> _kinectList;
};