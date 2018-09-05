#include "metaAiSymbol.h"

#pragma region SymbolNode
symbolNode::symbolNode()
	:_pos(0)
{
}

//---------------------------------------
void symbolNode::init(int x, int y)
{
	_pos.set(x, y);
}

//---------------------------------------
void symbolNode::update(float delta)
{
	_pos.z = ofNoise(_pos.x, _pos.y, ofGetElapsedTimef()) * 100.0f;
}

//---------------------------------------
void symbolNode::draw(float size)
{
	ofPushStyle();
	ofDrawSphere(_pos, size);
	ofPopStyle();
}

//---------------------------------------
ofVec3f symbolNode::getPos()
{
	return _pos;
}

#pragma endregion

#pragma region Sybmol
void symbol::load(string path)
{
	if (!_symbol.load(path))
	{
		ofLog(OF_LOG_ERROR, "[symbol::load]load failed");
		return;
	}

	auto pixel = _symbol.getPixels();
	for (int i = 0; i < cMetaAiSymbolNodeNum; i++)
	{
		uint8_t r = pixel[i * 3];
		uint8_t g = pixel[i * 3 + 1];
		uint8_t b = pixel[i * 3 + 2];

		int intensity = static_cast<int>((r + g + b) / 3.0f + 0.5f);
		if (intensity > cMetaAiPixelThreshold)
		{
			_symbolFlag[i] = true;
			_symbolColor[i] = ofColor(r, g, b);
		}
		else
		{
			_symbolFlag[i] = false;
		}
	}
	
}
#pragma endregion


#pragma region metaAi Symbol Display
//---------------------------------------
metaAiSymbolDisplay::metaAiSymbolDisplay()
	:_symbolRef(nullptr)
{}

//---------------------------------------
void metaAiSymbolDisplay::setup(int size, int range, float threshold)
{
	_displaySize = size;
	float half = _displaySize * 0.5f;
	_moveRange = range;
	_threshold = threshold * threshold;

	_symbolLine.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);

	float unitDist = _displaySize / (float)cMetaAiSymbolSize;
	float unitDistPow = unitDist * unitDist;
	_nodeSize = unitDist * 0.2f;

	initBaseDist(unitDist);

	int maxUnit = ceil(threshold / unitDist);
	for (int y = 0; y < cMetaAiSymbolSize; y++)
	{
		for (int x = 0; x < cMetaAiSymbolSize; x++)
		{
			int index = x + y * cMetaAiSymbolSize;
			_symbolLineCheck[index] = getNearIndex(x, y, maxUnit);
			_symbolNode[index].init(x * unitDist - half, y * unitDist - half);
		}
	}
}

//---------------------------------------
void metaAiSymbolDisplay::update(float delta)
{
	if (_symbolRef == nullptr)
	{
		return;
	}

	for (auto& iter : _symbolNode)
	{
		iter.update(delta);
	}
	updateLine();
}

//---------------------------------------
void metaAiSymbolDisplay::draw()
{
	if (_symbolRef == nullptr)
	{
		return;
	}
	ofPushStyle();
	
	//for (int i = 0; i < cMetaAiSymbolNodeNum; i++)
	//{
	//	if (!_symbolRef->_symbolFlag[i])
	//	{
	//		continue;
	//	}
	//	ofSetColor(_symbolRef->_symbolColor[i]);
	//	_symbolNode[i].draw(_nodeSize);
	//}
	
	_symbolLine.draw();
	ofPopStyle();
}

//---------------------------------------
void metaAiSymbolDisplay::setSymbol(symbol & data)
{
	_symbolRef = &data;
}

//---------------------------------------
void metaAiSymbolDisplay::updateLine()
{
	_symbolLine.clear();
	for (int i = 0; i < cMetaAiSymbolNodeNum; i++)
	{
		if (!_symbolRef->_symbolFlag[i])
		{
			continue;
		}

		int sX, sY;
		index2xy(i, sX, sY);

		//Update Line
		ofVec3f pos = _symbolNode[i].getPos();
		for (auto& iter : _symbolLineCheck[i])
		{
			if (iter <= i || !_symbolRef->_symbolFlag[iter])
			{
				//draw before
				continue;
			}
			int tX, tY;
			index2xy(iter, tX, tY);

			ofVec3f target = _symbolNode[iter].getPos();

			int dx = abs(sX - tX);
			int dy = abs(sY - tY);
			auto dist = _symbolBaseDist[dx] + _symbolBaseDist[dy] + (target.z - pos.z) * (target.z - pos.z);

			if (dist <= _threshold)
			{
				_symbolLine.addVertex(pos);
				_symbolLine.addVertex(target);
			}
		}
	}
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
vector<int> metaAiSymbolDisplay::getNearIndex(int x, int y, int maxNum)
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

#pragma endregion


