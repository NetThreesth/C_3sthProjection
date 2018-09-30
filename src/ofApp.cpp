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

	switch (key)
	{
	case 's':
	{
		_viewCam.start();
		break;
	}
	}
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
	_viewCam.update(delta);
}

//--------------------------------------------------------------
void ofApp::drawViewer()
{
	ofSetDepthTest(true);
	_cam.begin();
	_viewArms.draw(_armsPos);
	_viewThreeBody.draw(_threeBodyPos);
	_viewSymbol.draw(_symbolPos);


	//Debug
	_viewCam.draw();
	_cam.end();
	ofSetDepthTest(false);
}
