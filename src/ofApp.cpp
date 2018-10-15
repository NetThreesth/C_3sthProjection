#include "ofApp.h"
#include "config.h"

//--------------------------------------------------------------
void ofApp::setup() {

	config::getInstance()->init();
	serverReq::getInstance()->init();
	serverReq::getInstance()->reqChatCount();
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
	_animFadeAlpah.setDuration(config::getInstance()->_faderT);
	ofAddListener(_animFadeAlpah.animFinished, this, &ofApp::onFadeFinish);

	_bgm.load("bgm.mp3");
	_bgm.setLoop(true);
	_bgm.play();

	//Auto
	if (config::getInstance()->_exIsAutoLoop)
	{
		_loopTimer = config::getInstance()->_exLoopWaitT;
		_waitAutoStart = true;
	}
	_showDebug = _showGUI = false;
	ofHideCursor();
	ofBackground(0);
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
	serverReq::getInstance()->update();
	
	if (config::getInstance()->_exIsAutoLoop && _waitAutoStart)
	{
		_loopTimer -= delta;
		if (_loopTimer < 0.0f)
		{
			start();
			_waitAutoStart = false;
		}
	}

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
			ofClear(0);
			drawViewer();
		}
		_multiCam.end();
	}

	if (_showDebug && !config::getInstance()->_exOnlyMultiCam)
	{
		debugDraw();
	}
	ofSetDepthTest(false);
	_multiCam.draw();

	ofPushStyle();
	ofSetColor(0, _animFadeAlpah.getCurrentValue());
	ofDrawRectangle(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
	ofPopStyle();


	if (_showGUI)
	{
		config::getInstance()->draw();
	}

	//Debug
	//_kinectMgr.draw();
	_viewSymbol.debugDraw();
	//flowField::getInstance()->draw(cMetaballRect.x * 0.5f, cMetaballRect.y * 0.5, cMetaballRect.width *0.5, cMetaballRect.height*0.5);
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
		start();
		break;
	}
	case 'd':
	{
		_showDebug ^= true;
		break;
	}
	case 'g':
	{
		_showGUI ^= true;
		_showGUI ? ofShowCursor() : ofHideCursor();
		break;
	}
	}
}

//--------------------------------------------------------------
void ofApp::onFadeFinish(ofxAnimatable::AnimationEvent & e)
{
	if (_animFadeAlpah.getCurrentValue() == 0.0 && config::getInstance()->_exIsAutoLoop)
	{
		_loopTimer = config::getInstance()->_exLoopWaitT;
		_waitAutoStart = true;
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
		_viewParticle.stop();
		_animFadeAlpah.setDuration(config::getInstance()->_faderT);
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
		_viewSymbol.hideMirror();
		_viewThreeBody.stop();
		
		break;
	}
	case eSymbolToWait:
	{
		_animFadeAlpah.setDuration(config::getInstance()->_faderT);
		_animFadeAlpah.animateTo(255);
		break;
	}
	}
}

//--------------------------------------------------------------
void ofApp::onUpdateParticleNum(int & count)
{
	_particleNum = count;

	_viewThreeBody.setup(_particleNum);
}

//--------------------------------------------------------------
void ofApp::onSymbolFinish()
{
	_viewCam.symbolIsFinish();
}

//--------------------------------------------------------------
void ofApp::start()
{
	if (_viewCam.getState() == eViewWait)
	{
		if (_viewArms.start())
		{
			_viewCam.start();
			_viewParticle.start(_particleNum);
		}
	}
	else
	{
		ofLog(OF_LOG_ERROR, "[ofApp::start]Wrong State");
	}
}

//--------------------------------------------------------------
void ofApp::setupViewer()
{
	_viewArms.setup();
	
	_viewSymbol.setup(cSymbolRect.getWidth(), cSymbolRect.getHeight());

	_armsPos.set(0);
	_threeBodyPos.set(cThreeBodyPos);
	_symbolPos.set(cSymbolPos);
	_particlePos.set(cParticlePos);

	_particleNum = 200;//Default
	ofAddListener(serverReq::getInstance()->newChatCount, this, &ofApp::onUpdateParticleNum);

	_viewArms.setStage(true);

	ofAddListener(_viewCam._onViewStateChange, this, &ofApp::onViewerChange);
	ofAddListener(_viewSymbol._symbolPlayFinish, this, &ofApp::onSymbolFinish);
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
	start();
}

//--------------------------------------------------------------
void ofApp::onBlobOut(int& num)
{
	_viewSymbol.removeMetaball(num);
}

#pragma endregion
