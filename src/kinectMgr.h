#pragma once

#include "kinectUnit.h"


class kinectMgr
{
public:
	kinectMgr()
		:_blobCounter(0)
	{}
	void setup();
	void update(float delta);
	
	void draw();

private:
	void checkBlob(vector<trackBlob>& nextBlobList);
	void addNewBlob(blobData& blob, vector<trackBlob>& blobList);
	bool mergeCheck(vector<trackBlob>& blobList);
	void tracking(vector<trackBlob>& blobList);

public:
	int _blobCounter;
	vector<trackBlob> _mergeBlobList;
	array<kinectUnit, cKinectNum> _kinectList;

	vector<ofRectangle> _mergeArea;
};