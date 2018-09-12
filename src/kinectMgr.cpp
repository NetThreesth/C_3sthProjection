#include "kinectMgr.h"

//--------------------------------
void kinectMgr::setup()
{
	_kinectList[0].setup(1, 0, 0, 640, 480, 2233);
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
		merge();
	}
}

//--------------------------------
void kinectMgr::draw()
{
	ofPushStyle();
	ofNoFill();
	ofSetColor(255);
	for (auto& iter : _megreBlobList)
	{
		ofDrawRectangle(iter.x, iter.y, iter.width, iter.height);
	}
	ofPopStyle();
}

//--------------------------------
void kinectMgr::merge()
{
	_megreBlobList.clear();
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
			_megreBlobList.push_back(newBlob);
		}
	}
}
