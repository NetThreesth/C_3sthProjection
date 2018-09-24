#include "baseSymbol.h"


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
	_pos.z = (ofNoise(_pos.x, _pos.y, time * 0.1f) - 0.5) * range;
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

#pragma region symbolAnim
//---------------------------------------
symbolAnim::symbolAnim()
	:_animSymbol(nullptr)
	, _startAnim(false)
{}

//---------------------------------------
void symbolAnim::update(float delta)
{
	if (!_startAnim || _animSymbol == nullptr)
	{
		return;
	}

	bool isAllFinish = true;
	for (int i = 0; i < cMetaAiSymbolNodeNum; i++)
	{
		if (!_animSymbol->_symbolFlag[i])
		{
			continue;
		}
		_animList[i].update(delta);
		if (_animList[i].isAnimating())
		{
			_animSymbol->_symbolColor[i] = _animList[i].getCurrentColor();
		}
		auto isFinish = (_animList[i].hasFinishedAnimating() && _animList[i].getPercentDone() == 1.0f);
		if (isFinish)
		{
			if (_animSymbol->_symbolColor[i].a == 0.0f)
			{
				_animSymbol->_symbolFlag[i] = false;
			}

		}
		isAllFinish &= isFinish;
	}

	if (isAllFinish)
	{
		_startAnim = false;
		ofNotifyEvent(_onSymbolAnimFinish, this);
	}
	
}

//---------------------------------------
void symbolAnim::setSymbol(symbol * ptr)
{
	_animSymbol = ptr;
}

//---------------------------------------
bool symbolAnim::toSymbol(symbol & target, float duration)
{
	if (_animSymbol == nullptr)
	{
		ofLog(OF_LOG_ERROR, "[symbolAnim::toSymbol]symbol ptr is null");
		return false;
	}


	_startAnim = true;

	for (int i = 0; i < cMetaAiSymbolNodeNum; i++)
	{
		if (!_animSymbol->_symbolFlag[i] && !target._symbolFlag[i])
		{
			continue;
		}

		if (!_animSymbol->_symbolFlag[i])
		{
			_animSymbol->_symbolFlag[i] = true;
			_animSymbol->_symbolColor[i].set(0,0);
		}

		_animList[i].setDuration(duration);
		_animList[i].setColor(_animSymbol->_symbolColor[i]);
		if (!target._symbolFlag[i])
		{
			_animList[i].fadeOut();
		}
		else
		{
			_animList[i].animateTo(target._symbolColor[i]);
		}
	}
	return true;
}

#pragma endregion

