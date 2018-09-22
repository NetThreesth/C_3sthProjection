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
	_pos.z = (ofNoise(_pos.x, _pos.y) - 0.5) * 100;
}

//---------------------------------------
void symbolNode::update(float time, float range)
{
	_pos.z = (ofNoise(_pos.x, _pos.y, time) - 0.5) * range;
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
//---------------------------------------
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
			_symbolColor[i] = ofColor(r, g, b, ofRandom(30, 100));
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

	lightSetup();
	_mainTimer = 0.0f;
}

//---------------------------------------
void metaAiSymbolDisplay::update(float delta)
{
	if (_symbolRef == nullptr)
	{
		return;
	}

	lightUpdate();
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
	//lightEnable();
	ofSetColor(255);
	//drawNode();
	
	_symbolLine.draw();
	_symbolMesh.drawWireframe();
	_symbolMesh.draw();
	//lightDisable();
	ofPopStyle();
	ofSetDepthTest(false);

}

//---------------------------------------
void metaAiSymbolDisplay::setSymbol(symbol & data)
{
	_symbolRef = &data;
}

//---------------------------------------
void metaAiSymbolDisplay::toSymbol(symbol & toData)
{
	_symbolTarget = &toData;
	toCenter();
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
	case eAnimOutput:
	case eAnimIn:
	{
		for (auto& iter : _animPointList)
		{
			iter.update(delta);
		}
		_animTimer -= delta;
		if (_animTimer <= 0.0f)
		{
			if (_eState == eAnimOutput)
			{
				_eState = eAnimDisplay;

			}
			else
			{
				toTargetSymbol();
			}
		}
		else
		{
			updateLine();
		}
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

//---------------------------------------
void metaAiSymbolDisplay::updateLine()
{
	for (int i = 0; i < _animPointList.size(); i++)
	{
		_symbolLine.setVertex(i, _animPointList[i].getCurrentPosition());
	}
}

//---------------------------------------
void metaAiSymbolDisplay::toCenter()
{
	_eState = eAnimIn;
	_animPointList.clear();

	_animPointList.resize(_symbolLine.getNumVertices());
	for (int i = 0; i < _symbolLine.getNumVertices(); i++)
	{
		_animPointList[i].setPosition(_symbolLine.getVertex(i));
		_animPointList[i].setDuration(1.0f + ofRandom(-0.5, 0.5));

		_animPointList[i].animateTo(ofPoint(0));
	}
	_animTimer = 1.5f;
}

//---------------------------------------
void metaAiSymbolDisplay::toTargetSymbol()
{
	_eState = eAnimOutput;
	rebuildLine(_symbolTarget);
	_animPointList.clear();
	_animPointList.resize(_symbolLine.getNumVertices());
	for (int i = 0; i < _symbolLine.getNumVertices(); i++)
	{
		_animPointList[i].setPosition(ofPoint(0));
		_animPointList[i].setDuration(1.0f + ofRandom(-0.5, 0.5));

		_animPointList[i].animateTo(_symbolLine.getVertex(i));
		_symbolLine.setVertex(i, ofPoint(0));
	}
	_animTimer = 1.5f;
	_symbolRef = _symbolTarget;
	_symbolTarget = nullptr;
}

//---------------------------------------
void metaAiSymbolDisplay::lightSetup()
{
	_pointLight.setDiffuseColor(ofColor(64));

	// specular color, the highlight/shininess color //
	_pointLight.setSpecularColor(ofColor(255));
	_pointLight.setPointLight();


	_spotLight.setDiffuseColor(ofColor(255));
	_spotLight.setSpecularColor(ofColor(255));

	// turn the light into spotLight, emit a cone of light //
	_spotLight.setSpotlight();

	// size of the cone of emitted light, angle between light axis and side of cone //
	// angle range between 0 - 90 in degrees //
	_spotLight.setSpotlightCutOff(90);

	// rate of falloff, illumitation decreases as the angle from the cone axis increases //
	// range 0 - 128, zero is even illumination, 128 is max falloff //
	_spotLight.setSpotConcentration(128);


	// Directional Lights emit light based on their orientation, regardless of their position //
	_directionalLight.setDiffuseColor(ofColor(255.f));
	_directionalLight.setSpecularColor(ofColor(255.f));
	_directionalLight.setDirectional();

	// set the direction of the light
	_directionalLight.setOrientation(ofVec3f(0, 0, 0));
}

//---------------------------------------
void metaAiSymbolDisplay::lightUpdate()
{
	_spotLight.setPosition(600 * sin(ofGetElapsedTimef()), 0, 500 * cos(ofGetElapsedTimef()));
	_spotLight.lookAt(ofVec3f());

	_pointLight.setPosition(600 * (1 - sin(ofGetElapsedTimef())), 0, 600 * (1 - cos(ofGetElapsedTimef())));
	_pointLight.lookAt(ofVec3f());
}

//---------------------------------------
void metaAiSymbolDisplay::lightEnable()
{
	ofEnableLighting();
	_directionalLight.enable();
	_spotLight.enable();
	_pointLight.enable();
}

//---------------------------------------
void metaAiSymbolDisplay::lightDisable()
{
	_directionalLight.disable();
	_spotLight.disable();
	_pointLight.disable();
	ofDisableLighting();
}



#pragma endregion


