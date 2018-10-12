#include "metaball.h"
#include "flowField.h"
#include "config.h"

#pragma region metaNode
//--------------------------------------------------------------
void metaNode::setup(int x, int y, bool useBasicFlow)
{
	_pos.set(x, y);
	_vec.set(0);
	_acc.set(0);
	_useBasic = useBasicFlow;

	_r = cMetaballMaxSize * 0.5 * ofRandom(0.07f, 0.13f);
}

//--------------------------------------------------------------
void metaNode::update(float delta)
{

	_vec += _acc * delta;
	_pos += _vec * delta;

	if (_pos.x > cMetaballRect.width)
	{
		_pos.x = 0;
	}
	else if (_pos.x < 0)
	{
		_pos.x = cMetaballRect.width - 1;
	}
	if (_pos.y > cMetaballRect.height)
	{
		_pos.y = 0;
	}
	else if (_pos.y < 0)
	{
		_pos.y = cMetaballRect.height - 1;
	}

	ofVec2f flowForce;
	if (_useBasic)
	{
		flowForce = flowField::getInstance()->getBasicForce(_pos.x, _pos.y, cMetaballRect.width, cMetaballRect.height);
	}
	else
	{
		flowForce = flowField::getInstance()->getForce(_pos.x, _pos.y, cMetaballRect.width, cMetaballRect.height);
	}

	if (flowForce.length() > 0)
	{
		ofVec2f steer = flowForce - _vec;
		_acc = (steer * 50);
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

#pragma endregion

//--------------------------------------------------------------
void metaball::setup()
{
	_canvas.allocate(cMetaballRect.width, cMetaballRect.height, GL_RGBA);

	_tmp.allocate(cMetaballRect.width, cMetaballRect.height, ofImageType::OF_IMAGE_COLOR);
	if (!_meta.load("shader/metaShader"))
	{
		ofLog(OF_LOG_ERROR, "Load shader failed");
	}

	for (int i = 0; i < config::getInstance()->_exMetaballBasicNum; i++)
	{
		metaNode newNode;
		newNode.setup(ofRandom(0, cMetaballRect.width),
			ofRandom(0, cMetaballRect.height),
			true
		);
		_nodeBasicSet.push_back(newNode);

	}
}

//--------------------------------------------------------------
void metaball::update(float delta)
{
	nodeUpdate(delta);

	float p[cMetaballNum * 3];
	int flag[cMetaballNum] = { 0 };
	int idx = 0;
	for (auto& iter : _nodeBasicSet)
	{
		if (idx >= cMetaballNum)
		{
			break;
		}

		auto data = iter.getPosAndR();
		p[idx * 3] = data.x;
		p[idx * 3 + 1] = data.y;
		p[idx * 3 + 2] = data.z;
		flag[idx] = 255;
		idx++;
	}

	for (auto& iter : _nodeSet)
	{
		if (idx >= cMetaballNum)
		{
			break;
		}

		auto data = iter.getPosAndR();
		p[idx * 3] = data.x;
		p[idx * 3 + 1] = data.y;
		p[idx * 3 + 2] = data.z;
		flag[idx] = 255;
		idx++;
	}

	_canvas.begin();
	ofClear(255);
	_meta.begin();
	_meta.setUniform3fv("metaPos", p, cMetaballNum);
	_meta.setUniform1iv("metaFlag", flag, cMetaballNum);
	_meta.setUniform1i("metaWidth", cMetaballRect.width);
	_meta.setUniform1i("metaHeight", cMetaballRect.height);

	_tmp.draw(0, 0);
	_meta.end();
	_canvas.end();
}

//--------------------------------------------------------------
void metaball::draw()
{
	ofPushStyle();
	ofSetColor(255);
	_canvas.draw(0, 0);
	ofPopStyle();
}

//--------------------------------------------------------------
void metaball::drawNode()
{
	ofPushStyle();
	for (auto& iter : _nodeBasicSet)
	{
		iter.draw();
	}

	for (auto& iter : _nodeSet)
	{
		iter.draw();
	}
	ofPopStyle();
}

//--------------------------------------------------------------
void metaball::add(int num)
{
	int addNum = MIN(cMetaballNum - config::getInstance()->_exMetaballBasicNum - _nodeSet.size(), num);
	for (int i = 0; i < addNum; i++)
	{
		metaNode newNode;
		newNode.setup(ofRandom(0, cMetaballRect.width),
			ofRandom(0, cMetaballRect.height),
			false
		);
		_nodeSet.push_back(newNode);
	}
}

//--------------------------------------------------------------
void metaball::remove(int num)
{
	int removeNum = MIN(_nodeSet.size(), num);
	for (int i = 0; i < removeNum; i++)
	{
		_nodeSet.pop_front();
	}
}

//--------------------------------------------------------------
void metaball::nodeUpdate(float delta)
{
	for (auto& iter : _nodeBasicSet)
	{
		iter.update(delta);
	}

	for (auto& iter : _nodeSet)
	{
		iter.update(delta);
	}
}
