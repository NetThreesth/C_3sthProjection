#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	_bg.load("bg.jpg");
	_canvas.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);
	_tmp.allocate(ofGetWidth(), ofGetHeight(), ofImageType::OF_IMAGE_COLOR);
	if (!_meta.load("metaShader"))
	{
		ofLog(OF_LOG_ERROR, "Load shader failed");
	}
	ofBackground(0);
	for (auto& iter : _nodeSet)
	{
		iter.setup(ofRandomWidth(), ofRandomHeight());
	}
	_timer = ofGetElapsedTimef();
}

//--------------------------------------------------------------
void ofApp::update() {
	float delta = ofGetElapsedTimef() - _timer;
	_timer += delta;

	for (auto& iter : _nodeSet)
	{
		iter.update(delta);
	}
	drawOnCanvas();

	ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(255);
	_bg.draw(0, 0);

	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_MULTIPLY);
	_canvas.draw(0, 0);
	ofDisableBlendMode();

	//ofPushStyle();
	//for (auto& iter : _nodeSet)
	//{
	//	iter.draw();
	//}
	//ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::drawOnCanvas()
{
	float p[cMetaballNum * 3];
	int idx = 0;
	for (auto& iter : _nodeSet)
	{
		auto data = iter.getData();
		p[idx * 3] = data.x;
		p[idx * 3 + 1] = data.y;
		p[idx * 3 + 2] = data.z;
		idx++;
	}

	_canvas.begin();
	ofClear(0);
	_meta.begin();
	_meta.setUniform3fv("metaPos", p, cMetaballNum);
	_tmp.draw(0, 0);
	_meta.end();
	_canvas.end();
}

//--------------------------------------------------------------
void metaNode::setup(int x, int y)
{
	_pos.set(x, y);
	_vec.set(ofRandom(-200, 200), ofRandom(-200, 200));
	_r = ofRandom(20, 50);
}

//--------------------------------------------------------------
void metaNode::update(float delta)
{
	_pos += _vec * delta;

	if (_pos.x > ofGetWidth() || _pos.x < 0)
	{
		_vec.x *= -1;
	}
	if (_pos.y > ofGetHeight() || _pos.y < 0)
	{
		_vec.y *= -1;
	}
}

//--------------------------------------------------------------
void metaNode::draw()
{
	ofPushStyle();
	ofSetColor(255, 0, 0);
	ofNoFill();
	ofSetLineWidth(3);
	ofDrawCircle(_pos, _r);
	ofPopStyle();
}

//--------------------------------------------------------------
ofVec3f metaNode::getData()
{
	return ofVec3f(_pos.x, _pos.y, _r);
}
