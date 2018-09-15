#include "kinectMgr.h"
#include "flowField.h"

//--------------------------------
void kinectMgr::setup()
{
	_kinectList[0].setup(1, 10, 10, 640, 480, 2233);
	_kinectList[1].setup(2, 630, 10, 640, 480, 2244);
	_kinectList[2].setup(3, 10, 470, 640, 480, 2255);
	_kinectList[3].setup(4, 630, 470, 640, 480, 2266);

	_mergeArea[0].set((cKinectMergeRect.width - cKMergeSize) * 0.5, 0, cKMergeSize, cKinectMergeRect.height);
	_mergeArea[1].set(0, (cKinectMergeRect.height - cKMergeSize) * 0.5, cKinectMergeRect.width, cKMergeSize);
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
		//mergeCheck(newBlobList);
		
		tracking(newBlobList);
		_mergeBlobList = newBlobList;

		updateFlowField();
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
	ofSetColor(0, 255, 0);
	for (auto& iter : _mergeBlobList)
	{
		ofDrawRectangle(iter._rect);
		auto center = iter._rect.getCenter();
		ofLine(center, center - iter._vec);
		ofDrawBitmapString(ofToString(iter._bid), center);
	}

	//ofSetColor(255, 0, 0);
	//ofDrawRectangle(_mergeArea[0]);
	//ofDrawRectangle(_mergeArea[1]);
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

			addNewBlob(newBlob, nextBlobList);

		}
	}
}

//--------------------------------
void kinectMgr::addNewBlob(blobData& blob, vector<trackBlob>& blobList)
{
	bool independentBlob = true;
	ofRectangle newBlob(blob.x, blob.y, blob.width, blob.height);

	if (newBlob.intersects(_mergeArea[0]) || newBlob.intersects(_mergeArea[1]))
	{
		for (auto& iter : blobList)
		{
			ofRectangle oldBlob = iter._rect;
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
	}

	if (independentBlob)
	{
		blobList.push_back(blob);
	}

}

//--------------------------------
bool kinectMgr::mergeCheck(vector<trackBlob>& blobList)
{
	vector<trackBlob> megreList;
	megreList.push_back(blobList[0]);
	
	for (int i = 1; i < blobList.size(); i++)
	{
		bool independentBlob = true;
		for (auto& iter : megreList)
		{
			if (blobList[i]._rect.intersects(iter._rect))
			{
				ofVec2f p1, p2;
				p1.x = MIN(iter._rect.getMinX(), blobList[i]._rect.getMinX());
				p1.y = MIN(iter._rect.getMinY(), blobList[i]._rect.getMinY());
				p2.x = MAX(iter._rect.getMaxX(), blobList[i]._rect.getMaxX());
				p2.y = MAX(iter._rect.getMaxY(), blobList[i]._rect.getMaxY());

				iter._rect.set(p1, p2);

				independentBlob = false;
			}
		}

		if (independentBlob)
		{
			megreList.push_back(blobList[i]);
		}
	}

	if (megreList.size() == blobList.size())
	{
		return false;
	}
	else
	{
		blobList = megreList;
		return true;
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
	vector<bool> flagNewList, flagList;
	flagList.resize(_mergeBlobList.size(), false);
	flagNewList.resize(blobList.size(), false);
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
			if (flagNewList[j])
			{
				continue;
			}
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
			flagNewList[newIndex] = true;
			blobList[newIndex]._bid = _mergeBlobList[index]._bid;
			blobList[newIndex]._vec.set(blobList[newIndex]._rect.getCenter() - _mergeBlobList[index]._rect.getCenter());
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

//--------------------------------
void kinectMgr::updateFlowField()
{
	for (auto& iter : _mergeBlobList)
	{
		auto pos = iter._rect.getCenter();
		flowField::getInstance()->setForce(pos.x, pos.y, iter._vec, cKinectMergeRect.width, cKinectMergeRect.height);
	}
}
