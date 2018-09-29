#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	_viewSymbol.setup(cMetaballRect.getWidth(), cMetaballRect.getHeight());
	//_viewSymbol.start();

	//_kinectMgr.setup();
	_cam.setVFlip(true);
	ofSetSmoothLighting(true);

	ofBackground(50);
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
	
	_cam.begin();
	_viewSymbol.draw(ofVec3f(0, 0, 0));
	//_armKinect.draw();
	_cam.end();

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
