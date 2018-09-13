#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	_mb.setup(ofGetWidth(), ofGetHeight());
	initSymbol();
	_kinectMgr.setup();
	_cam.setVFlip(true);
	ofBackground(0);
	_timer = ofGetElapsedTimef();
}

//--------------------------------------------------------------
void ofApp::update() {
	float delta = ofGetElapsedTimef() - _timer;
	_timer += delta;

	_symbolDisplay.update(delta);
	_kinectMgr.update(delta);
	//_mb.update(delta);

	ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(255);

	_cam.begin();
	_symbolDisplay.draw();
	_cam.end();

	_kinectMgr.draw();

	//_mb.draw();
	//_mb.drawNode();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	switch (key)
	{
	case 'q':
	{
		_symbolIndex = (_symbolIndex + 1) % _symbolList.size();
		_symbolDisplay.toSymbol(_symbolList[_symbolIndex]);
		break;
	}
	}
}

//--------------------------------------------------------------
void ofApp::initSymbol()
{
	_symbolDisplay.setup(200, 100, 20);

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
