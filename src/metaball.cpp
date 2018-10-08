#include "metaball.h"
#include "flowField.h"

#pragma region metaNode
//--------------------------------------------------------------
void metaNode::setup(int x, int y, bool useBasicFlow)
{
	_pos.set(x, y);
	_vec.set(0);
	//_vec.set(ofRandom(-500, 500), ofRandom(-500, 500));
	_acc.set(0);
	_useBasic = _display = useBasicFlow;

	_r = cMetaballMaxSize * 0.5 * ofRandom(0.05f, 0.2f);
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
	else if(_pos.y < 0)
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

bool metaNode::isDisplay()
{
	return _useBasic || _display;
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

	for (int i = 0; i < _nodeSet.size(); i++)
	{
		if (i < cMetaballBasicFlowNum)
		{
			_nodeSet[i].setup(
				ofRandom(0, cMetaballRect.width),
				ofRandom(0, cMetaballRect.height),
				true
				);
		}
		else
		{
			_nodeSet[i].setup(
				ofRandom(0, cMetaballRect.width),
				ofRandom(0, cMetaballRect.height),
				false
			);
		}

	}
}

//--------------------------------------------------------------
void metaball::update(float delta)
{
	nodeUpdate(delta);

	float p[cMetaballNum * 3];
	int flag[cMetaballNum];
	int idx = 0;
	for (auto& iter : _nodeSet)
	{
		auto data = iter.getPosAndR();		
		p[idx * 3] = data.x;
		p[idx * 3 + 1] = data.y;
		p[idx * 3 + 2] = data.z;


		flag[idx] = iter.isDisplay()?255: 0;

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
	for (auto& iter : _nodeSet)
	{
		iter.draw();
	}
	ofPopStyle();
}

//--------------------------------------------------------------
void metaball::nodeUpdate(float delta)
{
	for (auto& iter : _nodeSet)
	{
		iter.update(delta);
	}
}
