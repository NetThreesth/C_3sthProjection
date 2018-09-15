#include "flowField.h"

//-----------------------------------
void flowField::forceUnit::update(float delta)
{
	_timer -= delta;
	if (_timer < 0.0f)
	{
		_needUpdate = false;
	}
}

//-----------------------------------
ofVec2f flowField::forceUnit::getForce()
{
	if (_needUpdate && _timer > 0.0f)
	{
		return _force * sin((_timer / cFFFourceTime) * HALF_PI);
	}
	else
	{
		return ofVec2f(0);
	}
	
}

//-----------------------------------
void flowField::forceUnit::setForce(ofVec2f force)
{
	_needUpdate = true;
	_force = force;
	_timer = cFFFourceTime;
}

//-----------------------------------
void flowField::update(float delta)
{
	for (auto& iter : _flowMap)
	{
		if (iter._needUpdate)
		{
			iter.update(delta);
		}
	}
}

//-----------------------------------
void flowField::draw(int x, int y, int width, int height)
{
	float unitW = (float)width / cFFColsNum;
	float unitH = (float)height / cFFRowsNum;


	ofPushStyle();
	{
		//Grid
		ofSetColor(255, 100);
		for (int i = 0; i <= cFFColsNum; i++)
		{
			ofLine(unitW * i, 0, unitW * i, height);
		}
		for (int i = 0; i <= cFFRowsNum; i++)
		{
			ofLine(0, unitH * i, width, unitH * i);
		}

		//flow
		ofSetColor(255);
		ofVec2f pos;
		for (int y = 0; y < cFFRowsNum; y++)
		{
			pos.y = unitH * 0.5 + unitH * y;
			for (int x = 0; x < cFFColsNum; x++)
			{
				int index = x + y * cFFColsNum;
				pos.x = unitW * 0.5 + unitW* x;
				ofLine(pos, pos + _flowMap[index].getForce());
			}
		}

	}
	ofPopStyle();
}

//--------------------------------------------------------------
void flowField::setForce(int x, int y, ofVec2f force, int width, int height)
{
	ofVec2f sp, ep;
	ep = getPos(x, y, width, height);
	sp = getPos(x - force.x, y - force.y, width, height);

	int sx = MIN(sp.x, ep.x);
	int sy = MIN(sp.y, ep.y);
	int ex = MAX(sp.x, ep.x);
	int ey = MAX(sp.y, ep.y);

	float unitW = (float)width / cFFColsNum;
	float unitH = (float)height / cFFRowsNum;

	for (int dy = sy; dy <= ey; dy++)
	{
		for (int dx = sx; dx <= ex; dx++)
		{
			ofRectangle rect;
			rect.set(dx * unitW, dy * unitH, unitW, unitH);

			if (rect.intersects(ofVec2f(x, y), ofVec2f(x - force.x, y - force.y)))
			{
				int index = dx + dy * cFFColsNum;
				_flowMap[index].setForce(force);
			}

		}
	}
}

//--------------------------------------------------------------
int flowField::getIndex(int x, int y, int width, int height)
{
	int indexX = floor(((float)x / width) * cFFColsNum);
	int indexY = floor(((float)y / height) * cFFRowsNum);

	return indexX + indexY * cFFColsNum;
}

//--------------------------------------------------------------
ofVec2f flowField::getPos(int x, int y, int width, int height)
{
	int indexX = floor(((float)x / width) * cFFColsNum);
	int indexY = floor(((float)y / height) * cFFRowsNum);
	return ofVec2f(indexX, indexY);
}

#pragma region Singleton
//--------------------------------------------------------------
flowField* flowField::pInstance = 0;

flowField* flowField::getInstance()
{
	if (pInstance == 0)
	{
		pInstance = new flowField();
	}
	return pInstance;
}

//--------------------------------------------------------------
void flowField::destroy()
{
	if (pInstance != 0)
	{
		delete pInstance;
	}
}
#pragma endregion


