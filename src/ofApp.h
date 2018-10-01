#pragma once

#include "constParameter.h"
#include "vArms.h"
#include "vSymbolMirror.h"
#include "vThreeBody.h"
#include "viewCam.h"
#include "kinectMgr.h"

class ofApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
		

private:
	float _timer;

	ofFbo _canvas;
	ofEasyCam _cam;


#pragma region Viewer
public:
	void onViewerChange(eViewState& nowState);
private:
	void setupViewer();
	void updateViewer(float delta);
	void drawViewer();
private:
	ofVec3f _symbolPos, _threeBodyPos, _armsPos;
	vSymbolMirror _viewSymbol;
	vThreeBody _viewThreeBody;
	vArms _viewArms;
	viewCam _viewCam;
#pragma endregion


#pragma region KinectMgr
private:
	kinectMgr _kinectMgr;
#pragma endregion



};

