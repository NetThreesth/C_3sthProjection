#include "vSymbolMirror.h"

//--------------------------------------------------------------
void vSymbolMirror::setup(int width, int height)
{

	_mb.setup();
	initSymbol();
	initLayerMask(width, height);
	reset();
	_isSetup = load("mirror");
}

//--------------------------------------------------------------
void vSymbolMirror::update(float delta)
{
	if (!_isSetup)
	{
		return;
	}

	_animMirrorAlpha.update(delta);
	_animSymbolAlpha.update(delta);
	updateMirror(delta);
	if (_isStart)
	{
		_symbolDisplay.update(delta);
		_mb.update(delta);

		updateToMirror();
		_mirrorContext.beginMask();
		ofSetColor(255);
		_mask.draw(0, 0, cMetaballRect.width, cMetaballRect.height);

		_mirrorContext.endMask();
	}
}

//--------------------------------------------------------------
void vSymbolMirror::draw(ofVec3f pos)
{
	if (!_isSetup)
	{
		return;
	}

	drawMirror(pos);
	drawSymbol(pos);
}

//--------------------------------------------------------------
void vSymbolMirror::reset()
{
	_animMirrorAlpha.reset(0);
	_animSymbolAlpha.reset(0);
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
void vSymbolMirror::displayMirror()
{
	_animMirrorAlpha.setDuration(3.0f);
	_animMirrorAlpha.animateFromTo(0.0, 1.0f);
}

//--------------------------------------------------------------
void vSymbolMirror::displayContent()
{
	_animSymbolAlpha.setDuration(2.0f);
	_animSymbolAlpha.animateFromTo(0.0, 255);
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
void vSymbolMirror::updateToMirror()
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

//--------------------------------------------------------------
void vSymbolMirror::drawSymbol(ofVec3f pos)
{
	if (!_isStart)
	{
		return;
	}

	ofPushMatrix();
	ofTranslate(pos);
	ofRotateX(90);
	ofPushStyle();
	{
		ofSetColor(255);
		_mirrorContext.draw(_mirrorContext.getWidth() * -0.5f, _mirrorContext.getHeight() * -0.5f);

		
	}	
	ofPopStyle();
	ofPopMatrix();
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
			//filter the point which it's alpha > 20
			if (c.a > cImg2MeshAlpahT)
			{
				c.a = 0.0f;
				ofVec3f vertex = ofVec3f((x - centerX) * 2, (y - centerY) * 2, float(brightness) / 255.0 * -50 + 25);
				_mirror.addVertex(vertex);
				_mirror.addColor(c);
			}
		}
	}
	_mirror.setMode(ofPrimitiveMode::OF_PRIMITIVE_POINTS);
	return true;
}

//--------------------------------------------------------------
void vSymbolMirror::updateMirror(float delta)
{
	if (!_animMirrorAlpha.isAnimating())
	{
		return;
	}

	for (int i = 0; i < _mirror.getNumColors(); i++)
	{
		auto color = _mirror.getColor(i);
		color.a = _animMirrorAlpha.getCurrentValue();
		_mirror.setColor(i, color);
	}
}

//--------------------------------------------------------------
void vSymbolMirror::drawMirror(ofVec3f pos)
{
	ofPushMatrix();
	ofTranslate(pos);
	ofRotateX(-90);
	ofPushStyle();
	{
		_mirror.draw();

	}
	ofPopStyle();
	ofPopMatrix();
}
#pragma endregion


