#include "metaAiSymbol.h"

#pragma region SymbolNode
metaAiSymbol::symbolNode::symbolNode()
	:_pos(0)
	,_color(0)
{
}

//---------------------------------------
void metaAiSymbol::symbolNode::init(int x, int y, ofColor c)
{
	_pos.set(x, y);
	_color = c;
}

//---------------------------------------
void metaAiSymbol::symbolNode::update(float delta)
{
}

//---------------------------------------
void metaAiSymbol::symbolNode::draw()
{
	ofPushStyle();
	ofSetColor(_color);
	ofDrawSphere(_pos, 20);
	ofPopStyle();
}

//---------------------------------------
ofVec3f metaAiSymbol::symbolNode::getPos()
{
	return _pos;
}

//---------------------------------------
ofColor metaAiSymbol::symbolNode::getColor()
{
	return _color;
}

#pragma endregion

//---------------------------------------
void metaAiSymbol::setup(int size, int range, float threshold)
{
	_displaySize = size;
	_moveRange = range;
	_threshold = threshold * threshold;

	float unitDist = _displaySize / (float)cMetaAiSymbolSize;
	unitDist *= unitDist;

	initBaseDist(unitDist);

	int maxUnit = ceil(_threshold / unitDist);
	for (int y = 0; y < cMetaAiSymbolSize; y++)
	{
		for (int x = 0; x < cMetaAiSymbolSize; x++)
		{
			int index = x + y * cMetaAiSymbolSize;
			_symbolLineCheck[index] = getNearIndex(x, y, maxUnit);
			_symbolFlag[index] = true;
						
			_symbolNode[index].init(x * unitDist, y * unitDist, ofColor(255));
		}
	}
}

//---------------------------------------
void metaAiSymbol::update(float delta)
{
	for (auto& iter : _symbolNode)
	{
		iter.update(delta);
	}
}

//---------------------------------------
void metaAiSymbol::draw()
{
	ofPushStyle();
	ofSetColor(255);
	for (int i = 0; i < cMetaAiSymbolNodeNum; i++)
	{
		int sX, sY;
		index2xy(i, sX, sY);
		//Draw Node
		_symbolNode[i].draw();
		
		//Draw Line
		ofVec3f pos = _symbolNode[i].getPos();
		for (auto& iter : _symbolLineCheck[i])
		{
			if (iter <= i)
			{
				//draw before
				continue;
			}
			int tX, tY;
			index2xy(iter, tX, tY);

			ofVec3f target = _symbolNode[iter].getPos();
			
			int diff = abs(sX - tX) + abs(sY - tY);
			auto dist = _symbolBaseDist[diff] + (target.z - pos.z) * (target.z - pos.z);
			
			if (dist <= _threshold)
			{
				ofLine(pos, target);
			}
		}
	}
	ofPopStyle();
}

//---------------------------------------
void metaAiSymbol::initBaseDist(float unit)
{

	for (int i = 0; i < cMetaAiSymbolSize * 2; i++)
	{
		_symbolBaseDist[i] = i * unit;
	}
}

//---------------------------------------
vector<int> metaAiSymbol::getNearIndex(int x, int y, int maxNum)
{
	vector<int> indexList;
	for (int dy = -maxNum; dy < maxNum; dy++)
	{
		for (int dx = -maxNum; dx < maxNum; dx++)
		{
			if (dy + dx > maxNum)
			{
				continue;
			}

			int indexX = x + dx;
			int indexY = y + dy;
			if(indexX >= 0 && indexX < cMetaAiSymbolSize && indexY >= 0 && indexY < cMetaAiSymbolSize)
			{
				int index = indexX + indexY * cMetaAiSymbolSize;
				indexList.push_back(index);
			}
		}
	}
	return indexList;
}

//---------------------------------------
void metaAiSymbol::index2xy(int index, int & x, int & y)
{
	x = index % cMetaAiSymbolSize;
	y = index / cMetaAiSymbolSize;
}
