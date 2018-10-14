#pragma once


#include "constParameter.h"
#include "vArms.h"
#include "vSymbolMirror.h"
#include "vThreeBody.h"
#include "viewCam.h"
#include "vParticleMgr.h"
#include "kinectMgr.h"
#include "multiCam.h"
#include "serverReq.h"

class ofApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();
	void debugDraw();

	void keyPressed(int key);
private:
	float _timer;
	multiCam _multiCam;
	ofEasyCam _cam;
	bool _showGUI, _showDebug;
	
private:
	void onFadeFinish(ofxAnimatable::AnimationEvent& e);

private:
	bool _isInFading;
	ofxAnimatableFloat _animFadeAlpah;


#pragma region Viewer
public:
	void onViewerChange(eViewState& nowState);
	void onUpdateParticleNum(int& count);
	void onSymbolFinish();
	void start();
private:
	void setupViewer();
	void updateViewer(float delta);
	void drawViewer();
private:
	int _particleNum;
	ofVec3f _symbolPos, _threeBodyPos, _armsPos, _particlePos;
	vSymbolMirror _viewSymbol;
	vThreeBody _viewThreeBody;
	vArms _viewArms;
	vParticleMgr _viewParticle;
	viewCam _viewCam;
#pragma endregion

#pragma region KinectMgr
public:
	void initKinect();
	void onNewBlobIn(int& num);
	void onBlobOut(int& num);
private:
	kinectMgr _kinectMgr;
#pragma endregion




};

