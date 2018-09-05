#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	
	_mb.setup(ofGetWidth(), ofGetHeight());

	for (int i = 0; i < cMetaAiSymbolDisplayNum; i++)
	{
		_symbolDisplay[i].setup(200, 100, 20);
		_symbolList[i].load("test" + ofToString(i+1) + ".png");
		_symbolDisplay[i].setSymbol(_symbolList[i]);
	}
	
	
	

	ofBackground(0);
	_timer = ofGetElapsedTimef();
}

//--------------------------------------------------------------
void ofApp::update() {
	float delta = ofGetElapsedTimef() - _timer;
	_timer += delta;

	for (int i = 0; i < cMetaAiSymbolDisplayNum; i++)
	{
		_symbolDisplay[i].update(delta);
	}
	
	//_mb.update(delta);

	ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(255);

	_cam.begin();
	int index = 0;
	for (int y = 0; y < 3; y++)
	{
		int pY = y * 250;
		for (int x = 0; x < 3; x++)
		{
			int pX = x * 250;
			ofPushMatrix();
			ofTranslate(pX, pY);

			_symbolDisplay[index].draw();
			ofPopMatrix();

			index++;
		}
	}
	
	_cam.end();

	//_mb.draw();
	//_mb.drawNode();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}
