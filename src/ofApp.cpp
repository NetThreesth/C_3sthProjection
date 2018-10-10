#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	config::getInstance()->init();
	setupViewer();

	if (config::getInstance()->_exOnlyMultiCam)
	{
		ofSetWindowShape(config::getInstance()->_exWindowWidth, config::getInstance()->_exWindowHeight);
		ofSetWindowPosition(0, 0);	
	}
	_multiCam.setup(0, 0, config::getInstance()->_exWindowWidth, config::getInstance()->_exWindowHeight);
	_multiCam.updateParent(_viewCam.getCam());

	initKinect();
	
	ofSetSmoothLighting(true);

	//Fade
	_animFadeAlpah.reset(0);
	_animFadeAlpah.setDuration(cViewFadeT);
	ofAddListener(_animFadeAlpah.animFinished, this, &ofApp::onFadeFinish);

	_showDebug = false;
	ofBackground(50);
	_timer = ofGetElapsedTimef();
}

//--------------------------------------------------------------
void ofApp::update() {
	float delta = ofGetElapsedTimef() - _timer;
	_timer += delta;

	
	updateViewer(delta);
	_multiCam.update(delta);
	_multiCam.updateParent(_viewCam.getCam());
	
	_animFadeAlpah.update(delta);
	
	_kinectMgr.update(delta);
	flowField::getInstance()->update(delta);
	
	
	//Debug
	_viewSymbol.debugUpdate(delta);
	ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw() {
	
	ofSetDepthTest(true);

	for (int i = 0; i < _multiCam.getCamNum(); i++)
	{
		_multiCam.begin(i);
		{
			drawViewer();
		}
		_multiCam.end();
	}

	if (_showDebug && !config::getInstance()->_exOnlyMultiCam)
	{
		debugDraw();
	}
	
	_multiCam.draw();
	ofSetDepthTest(false);

	ofPushStyle();
	ofSetColor(0, _animFadeAlpah.getCurrentValue());
	ofDrawRectangle(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
	ofPopStyle();


	//Debug
	//_kinectMgr.draw();
	_viewSymbol.debugDraw();
	flowField::getInstance()->draw(0, 0, cMetaballRect.width, cMetaballRect.height);
}

//--------------------------------------------------------------
void ofApp::debugDraw()
{
	_cam.begin();
	_viewArms.draw(_armsPos);
	_viewThreeBody.draw(_threeBodyPos);
	_viewSymbol.draw(_symbolPos);
	_viewParticle.draw(_particlePos);
	_viewCam.drawCamera();
	_multiCam.drawCam();
	_cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	switch (key)
	{
	case 's':
	{
		if (_viewArms.start())
		{
			_viewCam.start();
			_viewParticle.start(ofRandom(50, 100));
		}
		break;
	}
	case 'd':
	{
		_showDebug ^= true;
		break;
	}
	}
}

//--------------------------------------------------------------
void ofApp::onFadeFinish(ofxAnimatable::AnimationEvent & e)
{
	if (_animFadeAlpah.getCurrentValue() == 0.0 && config::getInstance()->_exIsAutoLoop)
	{
		if (_viewArms.start())
		{
			_viewCam.start();
			_viewParticle.start(50);
		}
	}
}

#pragma region Viewer
//--------------------------------------------------------------
void ofApp::onViewerChange(eViewState & nowState)
{
	switch (nowState)
	{
	case eViewWait:
	{
		_viewSymbol.stop();
		_viewSymbol.reset();
		_viewArms.setStage(true);
		_viewCam.reset();
		_animFadeAlpah.animateTo(0);
	}
	case eViewArms:
	{
		break;
	}
	case eArmsToThreeBody:
	{
		break;
	}
	case eViewThreeBody:
	{
		_viewSymbol.displayMirror();
		_viewThreeBody.start();

		_viewArms.stop();
		_viewArms.setStage(false);
		break;
	}
	case eViewThreeBodyAndSymbol:
	{
		_viewSymbol.start();
		break;
	}
	case eViewSymbol:
	{
		_viewThreeBody.stop();
		break;
	}
	case eSymbolToWait:
	{
		_animFadeAlpah.animateTo(255);

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
	_threeBodyPos.set(cThreeBodyPos);
	_symbolPos.set(cSymbolPos);
	_particlePos.set(cParticlePos);

	_viewArms.setStage(true);

	ofAddListener(_viewCam._onViewStateChange, this, &ofApp::onViewerChange);
}

//--------------------------------------------------------------
void ofApp::updateViewer(float delta)
{
	_viewArms.update(delta);
	_viewThreeBody.update(delta);
	_viewSymbol.update(delta);
	_viewParticle.update(delta);
	_viewCam.update(delta);
}

//--------------------------------------------------------------
void ofApp::drawViewer()
{
	ofPushStyle();
	glPointSize(2.0f);
	_viewArms.draw(_armsPos);
	_viewThreeBody.draw(_threeBodyPos);
	_viewSymbol.draw(_symbolPos);
	_viewParticle.draw(_particlePos);
	ofPopStyle();
}
#pragma endregion

#pragma region Kinect Manager
//--------------------------------------------------------------
void ofApp::initKinect()
{
	_kinectMgr.setup();

	ofAddListener(_kinectMgr._onNewBlobIn, this, &ofApp::onNewBlobIn);
	ofAddListener(_kinectMgr._onBlobOut, this, &ofApp::onBlobOut);
}

//--------------------------------------------------------------
void ofApp::onNewBlobIn(int& num)
{
	_viewSymbol.addMetaball(num);
}

//--------------------------------------------------------------
void ofApp::onBlobOut(int& num)
{
	_viewSymbol.removeMetaball(num);
}
#pragma endregion


