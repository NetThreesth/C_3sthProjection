#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	_viewSymbol.setup(ofGetWindowWidth(), ofGetWindowHeight());
	_viewSymbol.start();

	//_kinectMgr.setup();
	_cam.setVFlip(false);
	ofSetSmoothLighting(true);
	_timer = ofGetElapsedTimef();
}

//--------------------------------------------------------------
void ofApp::update() {
	float delta = ofGetElapsedTimef() - _timer;
	_timer += delta;

	_viewSymbol.update(delta);
	//_kinectMgr.update(delta);
	//_armKinect.update(delta);
	flowField::getInstance()->update(delta);
	ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(255);
	ofBackgroundGradient(ofColor(100), ofColor(15));
	//_cam.begin();
	_viewSymbol.draw();
	//_armKinect.draw();
	//_cam.end();

	//_kinectMgr.draw();
	//flowField::getInstance()->draw(0, 0, 1280, 960);

	ofDrawBitmapStringHighlight("Frame:" + ofToString(_armKinect.getFrame()), 0, 70);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	switch (key)
	{
	case 'k':
	{
		_armKinect.start();
		break;
	}
	}
}
