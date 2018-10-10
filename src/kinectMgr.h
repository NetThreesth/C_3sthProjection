#pragma once

#include "kinectUnit.h"


class kinectMgr
{
public:
	kinectMgr()
		:_blobCounter(0)
		, _timeout(-1)
	{}
	void setup();
	void update(float delta);
	
	void draw();

private:
	void checkBlob(vector<trackBlob>& nextBlobList);
	void addNewBlob(blobData& blob, vector<trackBlob>& blobList);
	bool mergeCheck(vector<trackBlob>& blobList);
	bool tracking(vector<trackBlob>& blobList);
	void updateFlowField();
private:
	int _blobCounter;
	float _timeout;
	vector<trackBlob> _mergeBlobList;
	array<kinectUnit, cKinectNum> _kinectList;

	array<ofRectangle, 2> _mergeArea;

//Evnet
public:
	ofEvent<int> _onNewBlobIn;
	ofEvent<int> _onBlobOut;
};