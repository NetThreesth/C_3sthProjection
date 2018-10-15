#include "vArms.h"
#include "config.h"

void vArms::setup()
{
	_arms.reset();
	_shiftPos.set(0, 0, 250);
	_isSetup = initStage();

	config::getInstance()->_ceilingAlpha.addListener(this, &vArms::onCeilingAlphaChange);
}

//------------------------------
void vArms::update(float delta)
{
	if (!_isSetup)
	{
		return;
	}

	if (_isStart)
	{
		_arms.update(delta);
	}
	updateStage(delta);
}

//------------------------------
void vArms::draw(ofVec3f pos)
{
	if (!_isSetup || !_displayStage)
	{
		return;
	}

	drawStage();
	_arms.draw(_shiftPos);
}

//------------------------------
bool vArms::start()
{
	if (_arms.play())
	{
		_isStart = true;
		return true;
	}
	else
	{
		return false;
	}
	
}

//------------------------------
void vArms::stop()
{
	_isStart = false;
	_arms.reset();
}

//------------------------------
void vArms::setStage(bool isDisplay)
{
	_displayStage = isDisplay;
}

#pragma region Stage

//------------------------------
bool vArms::initStage()
{
	bool result = true;
	result &= loadCeiling();
	result &= loadMirror(_mirrors[0], "stageMirror2");

	_spaceColor.set(255);
	_space.setRadius(cArmsSpaceSize);
	_space.setPosition(0, 0, 0);
	
	_rotD = 0.0f;
	_rotV = cArmsSpaceRotateV;

	return result;
}

//------------------------------
void vArms::updateStage(float delta)
{
	if (!_displayStage)
	{
		return;
	}

	_rotD += delta * _rotV;
	if (_rotD >= 360.0)
	{
		_rotD -= 360.0;
	}
}

//------------------------------
void vArms::drawStage()
{
	if (!_displayStage)
	{
		return;
	}
	ofPushStyle();

	//Ceiling
	ofPushMatrix();
	ofRotateY(_rotD);
	_ceiling.draw();
	ofPopMatrix();


	//Mirror 2
	ofPushMatrix();
	ofRotate(90, 1, 0, 0);
	ofTranslate(0, 0, 50);
	_mirrors[0].draw();
	ofPopMatrix();

	//Space
	ofPushMatrix();
	ofRotateY(-_rotD);
	ofSetColor(_spaceColor, config::getInstance()->_spaceAlpha);
	_space.drawWireframe();
	ofPopMatrix();

	ofPopStyle();
}

//------------------------------
bool vArms::loadCeiling()
{
	ofImage ceilingImg;
	if (!ceilingImg.load("image/algae.png"))
	{
		ofLog(OF_LOG_ERROR, "[vArms::loadCeiling]load celing image failed");
		return false;
	}

	_ceiling.clear();
	
	for (int y = 0; y < ceilingImg.getHeight(); y++) {
		float lon = ofMap(y, 0, ceilingImg.getHeight(), PI / 4, PI * 3 / 4);
		for (int x = 0; x < ceilingImg.getWidth(); x++) {
			float lat = ofMap(x, 0, ceilingImg.getWidth(), PI / 4, PI * 3 / 4);
			ofColor c = ceilingImg.getColor(x, y);
			int brightness = c.getBrightness();
			int calpha = c.a;
			//filter the point which it's alpha > 20
			if (calpha > cImg2MeshAlpahT) {
				float vx = cArmsSpaceSize *sin(lon)*cos(lat);
				float vy = cArmsSpaceSize *sin(lon)*sin(lat) + float(brightness) / 255.0 * -50;
				float vz = cArmsSpaceSize *cos(lon);
				ofVec3f vertex = ofVec3f(vx, vy, vz);
				c.a = config::getInstance()->_ceilingAlpha;
				_ceiling.addVertex(vertex);
				_ceiling.addColor(c);
			}
		}
	}
	_ceiling.setMode(ofPrimitiveMode::OF_PRIMITIVE_POINTS);
	return true;
}

//------------------------------
bool vArms::loadMirror(ofVboMesh& mesh, string name)
{
	mesh.clear();
	ofImage img;
	if (!img.load("image/" + name + ".png"))
	{
		ofLog(OF_LOG_ERROR, "[vArms::loadMirror]load mirror image failed : " + name);
		return false;
	}

	float centerX = img.getWidth() / 2;
	float centerY = img.getHeight() / 2;
	for (int y = 0; y < img.getHeight(); y++) {
		for (int x = 0; x < img.getWidth(); x++) {
			ofColor c = img.getColor(x, y);

			int brightness = c.getBrightness();
			if (c.a > cImg2MeshAlpahT) {
				ofVec3f vertex = ofVec3f((x - centerX), (y - centerY), float(brightness) / 255.0 * -50 + 25);
				c.a = cArmsMirrorAlpha;
				mesh.addVertex(vertex);
				mesh.addColor(ofColor(70));
			}
		}
	}
	mesh.setMode(ofPrimitiveMode::OF_PRIMITIVE_POINTS);
	return true;

}

//------------------------------
void vArms::updateCeilingAlpha(int alpha)
{
	for (int i = 0; i < _ceiling.getNumColors(); i++)
	{
		auto c = _ceiling.getColor(i);
		c.a = alpha / 255.0f;
		_ceiling.setColor(i, c);
	}
}

#pragma endregion

#pragma region GUI Event
//------------------------------
void vArms::onCeilingAlphaChange(int & alpha)
{
	updateCeilingAlpha(alpha);
}
#pragma endregion


