#include "flowField.h"
#include "config.h"
//-----------------------------------
void flowField::forceUnit::update(float delta)
{
	_timer -= delta;
	if (_timer < 0.0f)
	{
		_needUpdate = false;
		_force.set(0);
	}
}


//-----------------------------------
void flowField::forceUnit::updateBasicForce()
{
	float theta = ofMap(ofNoise(_vx, _vy, ofGetElapsedTimef() * 0.01), 0, 1, 0, TWO_PI);
	ofVec2f v(cos(theta), sin(theta));

	_basicForce.set(v * _basicForceSize * config::getInstance()->_metaballFieldBasic);

}

//-----------------------------------
ofVec2f flowField::forceUnit::getForce()
{
	if (_needUpdate && _timer > 0.0f)
	{
		return _force * sin((_timer / cFFForceTime) * HALF_PI);
	}
	else
	{
		return ofVec2f(0);
	}

}

//-----------------------------------
ofVec2f flowField::forceUnit::getBasicForce()
{
	return _basicForce;
}

//-----------------------------------
void flowField::forceUnit::setForce(ofVec2f force)
{
	_needUpdate = true;
	_force = force;
	_force.limit(cFFForceLimit);
	
	_timer = cFFForceTime;
}

//-----------------------------------
void flowField::update(float delta)
{
	for (auto& iter : _metaFlowMap)
	{
		if (iter._needUpdate)
		{
			iter.update(delta);
		}
		iter.updateBasicForce();
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
		ofSetColor(255, 0, 0, 100);
		for (int i = 0; i <= cFFColsNum; i++)
		{
			ofLine(x + unitW * i, y + 0, x + unitW * i, y + height);
		}
		for (int i = 0; i <= cFFRowsNum; i++)
		{
			ofLine(x, y + unitH * i, x + width, y + unitH * i);
		}

		//flow		
		ofVec2f pos;
		for (int dy = 0; dy < cFFRowsNum; dy++)
		{
			pos.y =  y + (unitH * 0.5 + unitH * dy);
			for (int dx = 0; dx < cFFColsNum; dx++)
			{
				int index = dx + dy * cFFColsNum;
				pos.x = x + (unitW * 0.5 + unitW * dx);

				ofSetColor(0, 255, 0);
				ofLine(pos, pos + _metaFlowMap[index].getForce());

				ofSetColor(0, 0, 255);
				ofLine(pos, pos + _metaFlowMap[index].getBasicForce());
			}
		}

	}
	ofPopStyle();
}

//--------------------------------------------------------------
void flowField::setForce(int x, int y, ofVec2f force, int width, int height)
{

	ofVec2f ep = getPos(x, y, width, height);
	ofVec2f sp = getPos(x - force.x, y - force.y, width, height);

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
				_metaFlowMap[index].setForce(force);
			}
		}
	}
}

//--------------------------------------------------------------
void flowField::setForce(ofRectangle range, ofVec2f force, int width, int height)
{
	ofVec2f sp = getPos(range.getLeft(), range.getTop(), width, height);
	ofVec2f ep = getPos(range.getRight(), range.getBottom(), width, height);


	for (int dy = sp.y; dy <= ep.y; dy++)
	{
		for (int dx = sp.x; dx <= ep.x; dx++)
		{
			int index = dx + dy * cFFColsNum;
			_metaFlowMap[index].setForce(force);
		}
	}

}

//--------------------------------------------------------------
ofVec2f flowField::getForce(int x, int y, int width, int height)
{
	return _metaFlowMap[getIndex(x, y, width, height)].getForce();
}

//--------------------------------------------------------------
ofVec2f flowField::getBasicForce(int x, int y, int width, int height)
{
	return _metaFlowMap[getIndex(x, y, width, height)].getBasicForce();
}

//--------------------------------------------------------------
int flowField::getIndex(int x, int y, int width, int height)
{
	int indexX = floor(((float)x / width) * cFFColsNum);
	int indexY = floor(((float)y / height) * cFFRowsNum);

	return MAX(MIN(indexX + indexY * cFFColsNum, _metaFlowMap.size() - 1), 0);
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


