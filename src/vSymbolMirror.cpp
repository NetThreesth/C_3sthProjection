#include "vSymbolMirror.h"

//--------------------------------------------------------------
void vSymbolMirror::setup(int width, int height)
{
	_mb.setup();
	initSymbol();
	initLayerMask(width, height);
	_isSetup = load("mirror");
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
	
	drawOnMirror();
	_mirrorContext.beginMask();
	ofSetColor(255);
	_mask.draw(0, 0, cMetaballRect.width, cMetaballRect.height);
	_mirrorContext.endMask();

}

//--------------------------------------------------------------
void vSymbolMirror::draw(ofVec3f pos)
{
	if (!_isSetup)
	{
		return;
	}

	ofPushMatrix();
	ofTranslate(pos);
	if (_isStart)
	{
		ofPushStyle();
		_mirrorContext.draw(_mirrorContext.getWidth() * -0.5f, _mirrorContext.getHeight() * -0.5f);
		ofPopStyle();
	}
	drawMirror();
	ofPopMatrix();
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
void vSymbolMirror::initLayerMask(int width, int height)
{
	_mirrorContext.setup(width, height);
	_mirrorContext.newLayer();
}

#pragma region Symbol & Metaball
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
#pragma endregion

//--------------------------------------------------------------
void vSymbolMirror::drawOnMirror()
{
	_mirrorContext.beginLayer();
	ofClear(255);
	ofPushStyle();
	ofSetColor(255);
	ofPushMatrix();
	ofTranslate(_mirrorContext.getWidth() * 0.5f, _mirrorContext.getHeight() * 0.5f);
	{
		_symbolDisplay.draw();
	}	
	ofPopMatrix();
	_mb.draw();
	ofPopStyle();
	_mirrorContext.endLayer();
}

#pragma region Mirror & Mask
//--------------------------------------------------------------
bool vSymbolMirror::load(string name)
{
	if (!_mask.load("image/" + name + "_mask.jpg"))
	{
		ofLog(OF_LOG_ERROR, "[vSymbolMirror::load]load mask faield");
		return false;
	}

	if (!loadMirror(name))
	{
		return false;
	}
	return true;
}

//--------------------------------------------------------------
bool vSymbolMirror::loadMirror(string name)
{
	ofImage mirror;
	if (!mirror.load("image/" + name + ".png"))
	{
		ofLog(OF_LOG_ERROR, "[vSymbolMirror::loadMirror]load mirror image faield");
		return false;
	}

	float centerX = mirror.getWidth() / 2;
	float centerY = mirror.getHeight() / 2;
	for (int y = 0; y < mirror.getHeight(); y += 2) {
		for (int x = 0; x < mirror.getWidth(); x += 2) {
			ofColor c = mirror.getColor(x, y);
			int brightness = c.getBrightness();
			int calpha = c.a;
			//filter the point which it's alpha > 20
			if (calpha > cMirrorImgAlphaT) 
			{
				ofVec3f vertex = ofVec3f((x - centerX) * 2, (y - centerY) * 2, float(brightness) / 255.0 * -50 + 25);
				_mirror.addVertex(vertex);
				_mirror.addColor(c);
			}
		}
	}
	return true;
}

//--------------------------------------------------------------
void vSymbolMirror::drawMirror()
{
	ofPushStyle();
	ofPushMatrix();
	_mirror.drawVertices();
	ofPopMatrix();
	ofPopStyle();
}
#pragma endregion


