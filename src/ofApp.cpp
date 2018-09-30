#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	setupViewer();
	//_kinectMgr.setup();
	//_cam.setVFlip(true);
	ofSetSmoothLighting(true);

	ofBackground(50);
	_timer = ofGetElapsedTimef();

	
}

//--------------------------------------------------------------
void ofApp::update() {
	float delta = ofGetElapsedTimef() - _timer;
	_timer += delta;

	updateViewer(delta);
	flowField::getInstance()->update(delta);
	ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw() {
	
	drawViewer();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	_viewArms.start();
}

//--------------------------------------------------------------
void ofApp::setupViewer()
{
	_viewArms.setup();
	_viewThreeBody.setup();
	_viewSymbol.setup(cMetaballRect.getWidth(), cMetaballRect.getHeight());

	_armsPos.set(0);
	_threeBodyPos.set(0, 5000, 0);
	_symbolPos.set(0, 5000, 0);

}

//--------------------------------------------------------------
void ofApp::updateViewer(float delta)
{
	_viewArms.update(delta);
	_viewThreeBody.update(delta);
	_viewSymbol.update(delta);
}

//--------------------------------------------------------------
void ofApp::drawViewer()
{
	_cam.begin();
	_viewArms.draw(_armsPos);
	_viewThreeBody.draw(_threeBodyPos);
	_viewSymbol.draw(_symbolPos);


	//Debug
	ofSetColor(255, 0, 0);
	ofDrawSphere(_armsPos, 10);
	ofSetColor(0, 255, 0);
	ofDrawSphere(_threeBodyPos, 10);
	ofSetColor(0, 0, 255);
	ofDrawSphere(_symbolPos, 10);
	_cam.end();
}
