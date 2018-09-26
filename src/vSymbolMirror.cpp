#include "vSymbolMirror.h"

//--------------------------------------------------------------
void vSymbolMirror::setup(int width, int height)
{
	_canvas.allocate(width, height, GL_RGB);
	_mb.setup(width, height);
	initSymbol();

	_isSetup = true;
}

//--------------------------------------------------------------
void vSymbolMirror::update(float delta)
{
	if (!_isSetup || !_isStart)
	{
		return;
	}

	_symbolDisplay.update(delta);
	_mb.update(delta);
	
	drawOnCanvas();
}

//--------------------------------------------------------------
void vSymbolMirror::draw()
{
	if (!_isSetup || !_isStart)
	{
		return;
	}

	ofPushStyle();
	_canvas.draw(0, 0);
	ofPopStyle();
}

//--------------------------------------------------------------
void vSymbolMirror::start()
{
	_isStart = true;
}

//--------------------------------------------------------------
void vSymbolMirror::stop()
{
	_isStart = false;
}

//--------------------------------------------------------------
void vSymbolMirror::initSymbol()
{
	_symbolDisplay.setup(640, 100, 11, 25);

	ofDirectory dir("symbol");
	dir.allowExt("png");
	auto size = dir.listDir();
	_symbolList.resize(size);
	for (int i = 0; i < size; i++)
	{
		_symbolList[i].load(dir.getPath(i));

	}
	_symbolDisplay.setSymbol(_symbolList[0]);
	_symbolIndex = 0;
}

//--------------------------------------------------------------
void vSymbolMirror::drawOnCanvas()
{
	_canvas.begin();
	ofClear(255);
	ofPushStyle();
	ofSetColor(255);

	_cam.begin();
	_symbolDisplay.draw();
	_cam.end();
	_mb.draw();
	//_mb.drawNode();
	ofPopStyle();
	_canvas.end();
}
