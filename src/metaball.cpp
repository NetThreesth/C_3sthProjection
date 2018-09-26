#include "metaball.h"
#include "flowField.h"

#pragma region metaNode
//--------------------------------------------------------------
void metaNode::setup(int x, int y, ofColor c)
{
	_color.set(c);
	_pos.set(x, y);
	//_vec.set(0);
	_vec.set(ofRandom(-500, 500), ofRandom(-500, 500));
	_acc.set(0);
	_r = ofRandom(50, 100);
}

//--------------------------------------------------------------
void metaNode::update(float delta)
{
	_vec += _acc * delta;
	_pos += _vec * delta;
	if (_pos.x > cMetaballRect.width)
	{
		_pos.x = cMetaballRect.width - 1;
		_vec.x *= -1;
	}
	else if (_pos.x < 0)
	{
		_pos.x = 0;
		_vec.x *= -1;
	}
	if (_pos.y > cMetaballRect.height)
	{
		_pos.y = cMetaballRect.height - 1;
		_vec.y *= -1;
	}
	else if(_pos.y < 0)
	{
		_pos.y = 0;
		_vec.y *= -1;
	}

	ofVec2f flowForce = flowField::getInstance()->getForce(_pos.x, _pos.y, cMetaballRect.width, cMetaballRect.height);
	if (flowForce.length() > 0)
	{
		ofVec2f steer = flowForce - _vec;
		_acc = (steer * 100);
	}
	else
	{
		_acc.set(0);
	}
}

//--------------------------------------------------------------
void metaNode::draw()
{
	ofPushStyle();
	ofSetColor(0);
	ofNoFill();
	ofSetLineWidth(3);
	ofDrawCircle(_pos, _r);
	ofPopStyle();
}

//--------------------------------------------------------------
ofVec3f metaNode::getPosAndR()
{
	return ofVec3f(_pos.x, _pos.y, _r);
}

//--------------------------------------------------------------
ofColor metaNode::getColor()
{
	return _color;
}
#pragma endregion

//--------------------------------------------------------------
void metaball::setup(int width, int height)
{
	_canvas.allocate(cMetaballRect.width, cMetaballRect.height, GL_RGBA);

	_tmp.allocate(width, height, ofImageType::OF_IMAGE_COLOR);
	if (!_meta.load("shader/metaShader"))
	{
		ofLog(OF_LOG_ERROR, "Load shader failed");
	}

	ofColor color(255, 0, 0);

	for (auto& iter : _nodeSet)
	{

		iter.setup(
			ofRandom(0, cMetaballRect.width),
			ofRandom(0, cMetaballRect.height),
			color
		);
		color.setHueAngle(color.getHueAngle() + ofRandom(30, 50));
	}
}

//--------------------------------------------------------------
void metaball::update(float delta)
{
	nodeUpdate(delta);

	float p[cMetaballNum * 3];
	float c[cMetaballNum * 4];
	int idx = 0;
	for (auto& iter : _nodeSet)
	{
		auto data = iter.getPosAndR();		
		p[idx * 3] = data.x;
		p[idx * 3 + 1] = data.y;
		p[idx * 3 + 2] = data.z;

		auto color = iter.getColor();
		c[idx * 4] = color.r / 255.0f;
		c[idx * 4 + 1] = color.g / 255.0f;
		c[idx * 4 + 2] = color.b / 255.0f;
		c[idx * 4 + 3] = color.a / 255.0f;

		idx++;
	}

	_canvas.begin();
	ofClear(255);
	_meta.begin();
	_meta.setUniform3fv("metaPos", p, cMetaballNum);
	_meta.setUniform4fv("metaColor", c, cMetaballNum);

	_tmp.draw(0, 0);
	_meta.end();
	_canvas.end();
}

void metaball::draw()
{
	ofPushStyle();
	ofSetColor(255);
	_canvas.draw(0, 0);
	ofPopStyle();
}

void metaball::drawNode()
{
	ofPushStyle();
	for (auto& iter : _nodeSet)
	{
		
		iter.draw();
	}
	ofPopStyle();
}

void metaball::nodeUpdate(float delta)
{
	for (auto& iter : _nodeSet)
	{
		iter.update(delta);
	}
}
