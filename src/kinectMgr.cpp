#include "kinectMgr.h"

//--------------------------------
void kinectMgr::setup()
{
	_kinectList[0].setup(1, 10, 10, 640, 480, 2233);
	_kinectList[1].setup(2, 630, 10, 640, 480, 2244);
	_kinectList[2].setup(3, 10, 470, 640, 480, 2255);
	_kinectList[3].setup(4, 630, 470, 640, 480, 2266);
}

//--------------------------------
void kinectMgr::update(float delta)
{
	bool hasUpdate = false;
	for (auto& iter : _kinectList)
	{
		hasUpdate |= iter.update(delta);
	}

	if (hasUpdate)
	{
		checkBlob();
	}
}

//--------------------------------
void kinectMgr::draw()
{
	ofPushStyle();
	ofNoFill();
	ofSetColor(255);
	for (auto& iter : _mergeBlobList)
	{
		ofDrawRectangle(iter.x, iter.y, iter.width, iter.height);
	}
	ofPopStyle();
}

//--------------------------------
void kinectMgr::checkBlob()
{
	_mergeBlobList.clear();
	for (auto& iter : _kinectList)
	{
		for (auto& blob : iter._blobList)
		{
			if (blob.width <= 0 || blob.height <= 0) {
				continue;
			}
			auto newBlob = blob;
			newBlob.x += iter._range.x;
			newBlob.y += iter._range.y;

			checkMerge(newBlob);

		}
	}
}

//--------------------------------
void kinectMgr::checkMerge(blobData& blob)
{
	bool independentBlob = true;
	
	for (auto& iter : _mergeBlobList)
	{
		ofRectangle oldBlob(iter.x, iter.y, iter.width, iter.height);
		ofRectangle newBlob(blob.x, blob.y, blob.width, blob.height);

		if (oldBlob.intersects(newBlob))
		{
			ofVec2f p1, p2;
			p1.x = MIN(oldBlob.getMinX(), newBlob.getMinX());
			p1.y = MIN(oldBlob.getMinY(), newBlob.getMinY());
			p2.x = MAX(oldBlob.getMaxX(), newBlob.getMaxX());
			p2.y = MAX(oldBlob.getMaxY(), newBlob.getMaxY());


			iter.x = p1.x;
			iter.y = p1.y;
			iter.width = p2.x - p1.x;
			iter.height = p2.y - p1.y;

			independentBlob = false;
			break;
		}
	}

	if (independentBlob)
	{
		_mergeBlobList.push_back(blob);
	}
	
}
