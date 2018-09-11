#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	
	_mb.setup(ofGetWidth(), ofGetHeight());
	initSymbol();

	
	

	ofBackground(0);
	_timer = ofGetElapsedTimef();
}

//--------------------------------------------------------------
void ofApp::update() {
	float delta = ofGetElapsedTimef() - _timer;
	_timer += delta;

	_symbolDisplay.update(delta);
	
	//_mb.update(delta);

	ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(255);

	_cam.begin();
	int index = 0;
	_symbolDisplay.draw();
	_cam.end();

	//_mb.draw();
	//_mb.drawNode();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

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
}
