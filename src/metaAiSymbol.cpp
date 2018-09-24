#include "metaAiSymbol.h"


#pragma region metaAi Symbol Display
//---------------------------------------
metaAiSymbolDisplay::metaAiSymbolDisplay()
	:_symbolRef(nullptr)
	, _eState(eAnimDisplay)
{}

//---------------------------------------
void metaAiSymbolDisplay::setup(int size, int range, float thresholdMin, float thresholdMax)
{
	_displaySize = size;
	float half = _displaySize * 0.5f;
	_moveRange = range;
	_tMax = thresholdMax * thresholdMax;
	_tMin = thresholdMin * thresholdMin;

	_symbolLine.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
	_symbolMesh.setMode(ofPrimitiveMode::OF_PRIMITIVE_TRIANGLES);

	float unitDist = _displaySize / (float)cMetaAiSymbolSize;
	float unitDistPow = unitDist * unitDist;
	_nodeSize = unitDist * 0.2f;


	initBaseDist(unitDist);

	int maxUnit = ceil(thresholdMax / unitDist);
	int minUnit = ceil(thresholdMin / unitDist);
	for (int y = 0; y < cMetaAiSymbolSize; y++)
	{
		for (int x = 0; x < cMetaAiSymbolSize; x++)
		{
			int index = x + y * cMetaAiSymbolSize;
			_symbolLineCheck[index] = getPossibleIndex(x, y, minUnit, maxUnit);
			_symbolNode[index].init(x * unitDist - half, y * unitDist - half);
		}
	}
	_mainTimer = 0.0f;
}

//---------------------------------------
void metaAiSymbolDisplay::update(float delta)
{
	if (_symbolRef == nullptr)
	{
		return;
	}
	
	_animSymbol.update(delta);
	checkState(delta);

}

//---------------------------------------
void metaAiSymbolDisplay::draw()
{
	if (_symbolRef == nullptr)
	{
		return;
	}

	ofSetDepthTest(true);
	ofPushStyle();
	ofSetColor(255);
	//drawNode();
	
	_symbolLine.draw();
	_symbolMesh.drawWireframe();
	_symbolMesh.draw();
	ofPopStyle();
	ofSetDepthTest(false);

}

//---------------------------------------
void metaAiSymbolDisplay::setSymbol(symbol & data)
{
	_symbolRef = &data;
	_animSymbol.setSymbol(_symbolRef);
}

//---------------------------------------
void metaAiSymbolDisplay::toSymbol(symbol & toData)
{
	//_symbolTarget = &toData;
	_animSymbol.toSymbol(toData, 3);
}

//---------------------------------------
void metaAiSymbolDisplay::drawNode()
{
	ofPushStyle();
	for (int i = 0; i < cMetaAiSymbolNodeNum; i++)
	{
		if (!_symbolRef->_symbolFlag[i])
		{
			continue;
		}
		ofSetColor(_symbolRef->_symbolColor[i]);
		_symbolNode[i].draw(_nodeSize);
	}
	ofPopStyle();
}

//---------------------------------------
void metaAiSymbolDisplay::rebuildLine(symbol* data)
{
	_symbolLine.clear();
	for (int i = 0; i < cMetaAiSymbolNodeNum; i++)
	{
		if (!data->_symbolFlag[i])
		{
			continue;
		}

		ofVec3f pos = _symbolNode[i].getPos();
		auto nearList = getNearIndex(data, i);

		//Update Line
		for (auto& iter : nearList)
		{
			_symbolLine.addVertex(pos);
			_symbolLine.addVertex(_symbolNode[iter].getPos());
			_symbolLine.addColor(data->_symbolColor[i]);
			_symbolLine.addColor(data->_symbolColor[iter]);

		}
	}
}

//---------------------------------------
void metaAiSymbolDisplay::rebuildMesh(symbol * data)
{
	_symbolMesh.clear();
	for (int i = 0; i < cMetaAiSymbolNodeNum; i++)
	{
		if (!data->_symbolFlag[i])
		{
			continue;
		}

		ofVec3f pos = _symbolNode[i].getPos();
		auto nearList = getNearIndex(data, i);
		set<int> nearSet;
		for (auto& iter : nearList)
		{
			nearSet.insert(iter);
		}

		for (auto& nearNode : nearList)
		{
			auto nearList2 = getNearIndex(data, nearNode);
			for (auto& nearNode2 : nearList2)
			{
				if (i < nearNode && nearSet.find(nearNode2) != nearSet.end())
				{
					auto pos2 = _symbolNode[nearNode].getPos();
					auto pos3 = _symbolNode[nearNode2].getPos();
					_symbolMesh.addVertex(pos);
					_symbolMesh.addVertex(pos2);
					_symbolMesh.addVertex(pos3);
					_symbolMesh.addColor(data->_symbolColor[i]);
					_symbolMesh.addColor(data->_symbolColor[nearNode]);
					_symbolMesh.addColor(data->_symbolColor[nearNode2]);
					break;
				}
			}

		}
	}
}

//---------------------------------------
vector<int> metaAiSymbolDisplay::getNearIndex(symbol * data, int index)
{
	vector<int> nearList;
	ofVec3f pos = _symbolNode[index].getPos();
	int sX, sY;
	index2xy(index, sX, sY);

	for (auto& iter : _symbolLineCheck[index])
	{
		if (iter <= index || !data->_symbolFlag[iter])
		{
			//check before
			continue;
		}
		int tX, tY;
		index2xy(iter, tX, tY);

		ofVec3f target = _symbolNode[iter].getPos();

		int dx = abs(sX - tX);
		int dy = abs(sY - tY);
		auto dist = _symbolBaseDist[dx] + _symbolBaseDist[dy] + (target.z - pos.z) * (target.z - pos.z);

		if (dist <= _tMax && dist >= _tMin)
		{
			nearList.push_back(iter);
		}
	}
	return nearList;
}

//---------------------------------------
void metaAiSymbolDisplay::initBaseDist(float unit)
{
	for (int i = 0; i < cMetaAiSymbolSize * 2; i++)
	{
		_symbolBaseDist[i] = (i * unit) * (i * unit);
	}
}

//---------------------------------------
vector<int> metaAiSymbolDisplay::getPossibleIndex(int x, int y, int minNum, int maxNum)
{
	vector<int> indexList;
	for (int dy = -maxNum; dy < maxNum; dy++)
	{
		for (int dx = -maxNum; dx < maxNum; dx++)
		{
			int val = abs(dy) + abs(dx);
			if (val > maxNum || val < minNum)
			{
				continue;
			}

			int indexX = x + dx;
			int indexY = y + dy;
			if (indexX >= 0 && indexX < cMetaAiSymbolSize && indexY >= 0 && indexY < cMetaAiSymbolSize)
			{
				int index = indexX + indexY * cMetaAiSymbolSize;
				indexList.push_back(index);
			}
		}
	}
	return indexList;
}

//---------------------------------------
void metaAiSymbolDisplay::index2xy(int index, int & x, int & y)
{
	x = index % cMetaAiSymbolSize;
	y = index / cMetaAiSymbolSize;
}

//---------------------------------------
void metaAiSymbolDisplay::checkState(float delta)
{
	switch (_eState)
	{
	case eAnimTranslate:
	{
		_animSymbol.update(delta);
		break;
	}
	case eAnimDisplay:
	{
		_mainTimer += delta;
		for (auto& iter : _symbolNode)
		{
			iter.update(_mainTimer, _moveRange);
		}
		rebuildLine(_symbolRef);
		rebuildMesh(_symbolRef);
		break;
	}
	}
}

#pragma endregion


