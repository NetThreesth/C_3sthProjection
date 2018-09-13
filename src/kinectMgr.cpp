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
	bool hasUpdate = true;
	for (auto& iter : _kinectList)
	{
		hasUpdate &= iter.update(delta);
	}

	if (hasUpdate)
	{
		vector<trackBlob> newBlobList;
		checkBlob(newBlobList);
		tracking(newBlobList);
		_mergeBlobList = newBlobList;

		for (auto& iter : _kinectList)
		{
			iter._isUpdate = false;
		}
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
		ofDrawRectangle(iter._rect);

		ofDrawBitmapStringHighlight(ofToString(iter._bid), iter._rect.getCenter());
	}
	ofPopStyle();
}

//--------------------------------
void kinectMgr::checkBlob(vector<trackBlob>& nextBlobList)
{
	nextBlobList.clear();

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

			checkMerge(newBlob, nextBlobList);

		}
	}
}

//--------------------------------
void kinectMgr::checkMerge(blobData& blob, vector<trackBlob>& blobList)
{
	bool independentBlob = true;

	for (auto& iter : blobList)
	{
		ofRectangle oldBlob = iter._rect;
		ofRectangle newBlob(blob.x, blob.y, blob.width, blob.height);

		if (oldBlob.intersects(newBlob))
		{
			ofVec2f p1, p2;
			p1.x = MIN(oldBlob.getMinX(), newBlob.getMinX());
			p1.y = MIN(oldBlob.getMinY(), newBlob.getMinY());
			p2.x = MAX(oldBlob.getMaxX(), newBlob.getMaxX());
			p2.y = MAX(oldBlob.getMaxY(), newBlob.getMaxY());


			iter._rect.x = p1.x;
			iter._rect.y = p1.y;
			iter._rect.width = p2.x - p1.x;
			iter._rect.height = p2.y - p1.y;

			independentBlob = false;
			break;
		}
	}

	if (independentBlob)
	{
		blobList.push_back(blob);
	}

}

//--------------------------------
void kinectMgr::tracking(vector<trackBlob>& blobList)
{
	if (blobList.size() == 0)
	{
		//No blob
		_blobCounter = 0;
		return;
	}

	if (_mergeBlobList.size() == 0)
	{
		//New Tracking
		for (auto& iter : blobList)
		{
			iter._bid = _blobCounter;
			_blobCounter++;
		}
		return;
	}

	vector<vector<blobDistSet>> distMap;
	vector<bool> flagList;
	flagList.resize(_mergeBlobList.size(), false);
	for (auto& newBlob : blobList)
	{
		vector<blobDistSet> dist;
		for (int i = 0; i < _mergeBlobList.size(); i++)
		{
			blobDistSet dSet(i, newBlob._rect.getCenter().distanceSquared(_mergeBlobList[i]._rect.getCenter()));
			dist.push_back(dSet);
		}
		sort(dist.begin(), dist.end(),
			[](const blobDistSet& a, const blobDistSet& b)
		{
			return a._dist < b._dist;
		}
		);
		distMap.push_back(dist);
	}

	for (int i = 0; i < blobList.size(); i++)
	{
		int index = -1;
		int newIndex = -1;
		float dist = cKTrackingMaxDist;
		for (int j = 0; j < distMap.size(); j++)
		{
			for (int k = 0; k < distMap[j].size(); k++)
			{
				if (!flagList[distMap[j][k]._id])
				{
					if (distMap[j][k]._dist < dist)
					{
						dist = distMap[j][k]._dist;
						index = distMap[j][k]._id;
						newIndex = j;
					}
					break;
				}
			}
		}

		if (index != -1)
		{
			flagList[index] = true;
			blobList[newIndex]._bid = _mergeBlobList[index]._bid;
		}		
	}

	for (auto& iter : blobList)
	{
		if (iter._bid == -1)
		{
			iter._bid = _blobCounter;
			_blobCounter++;
		}
	}

}
