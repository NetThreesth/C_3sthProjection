#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	
	_mb.setup(ofGetWidth(), ofGetHeight());

	_symbol.setup(640, 100, 20);

	ofBackground(0);
	_timer = ofGetElapsedTimef();
}

//--------------------------------------------------------------
void ofApp::update() {
	float delta = ofGetElapsedTimef() - _timer;
	_timer += delta;

	_symbol.update(delta);
	_mb.update(delta);

	ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(255);

	_cam.begin();
	_symbol.draw();
	_cam.end();

	//_mb.draw();
	//_mb.drawNode();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}
