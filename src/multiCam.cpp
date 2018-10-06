#include "multiCam.h"

#pragma region camUnit
//---------------------------------
multiCam::camUnit::camUnit()
	:_setBegin(false)
{
	_cam.disableMouseInput();
	_canvas.allocate(cMCCamCanvasWidth, cMCCamCanvasHeight, GL_RGB);
}

//---------------------------------
void multiCam::camUnit::update(float delta)
{
}

//---------------------------------
void multiCam::camUnit::draw(int x, int y, int width, int height)
{
	_canvas.draw(x, y, width, height);
}

//---------------------------------
void multiCam::camUnit::begin()
{
	if (_setBegin)
	{
		ofLog(OF_LOG_ERROR, "[multiCam::camUnit::begin]Unmatch end");
		return;
	}
	_setBegin = true;
	_canvas.begin();
	ofClear(50);
	_cam.begin();
}

//---------------------------------
void multiCam::camUnit::end()
{
	_cam.end();
	_canvas.end();
	_setBegin = false;
}
#pragma endregion


//---------------------------------
multiCam::multiCam()
	:_isSetup(false)
	, _drawCamIdx(-1)
{
}

//---------------------------------
void multiCam::setup(int x, int y, int width, int height)
{
	_displayRect.set(x, y, width, height);
	_unitW = (float)width / cMCCamNum;
	_unitH = height;

	float theata = 360 / cMCCamNum;
	ofVec2f v(0, cMCCamDist);
	for (int i = 0; i < cMCCamNum; i++)
	{
		_camList[i]._cam.setParent(_parent);
		_camList[i]._cam.setPosition(v);
		v.rotate(theata);
	}
}

//---------------------------------
void multiCam::update(float delta)
{
	for (auto& iter : _camList)
	{
		iter.update(delta);
	}
}

//---------------------------------
void multiCam::updateParent(ofNode & p)
{
	_parent = p;
}

//---------------------------------
void multiCam::draw()
{
	ofPushStyle();
	ofSetColor(255);
	ofPushMatrix();
	ofTranslate(_displayRect.getTopLeft());
	{
		for (int i = 0; i < cMCCamNum; i++)
		{
			_camList[i].draw(i * _unitW, 0, _unitW, _unitH);
		}
	}
	ofPopMatrix();
	ofPopStyle();
}

//---------------------------------
void multiCam::drawCam()
{
	ofPushStyle();
	for (auto& iter : _camList)
	{
		iter._cam.draw();
	}
	ofPopStyle();
}

//---------------------------------
int multiCam::getCamNum()
{
	return _camList.size();
}

//---------------------------------
void multiCam::begin(int i)
{
	if (_drawCamIdx == -1)
	{
		_camList[i].begin();
		_drawCamIdx = i;
	}
}

//---------------------------------
void multiCam::end()
{
	if (_drawCamIdx != -1)
	{
		_camList[_drawCamIdx].end();
		_drawCamIdx = -1;
	}	
}
