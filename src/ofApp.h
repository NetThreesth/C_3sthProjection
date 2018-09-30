#pragma once

#include "constParameter.h"
#include "vArms.h"
#include "vSymbolMirror.h"
#include "vThreeBody.h"

#include "kinectMgr.h"
class ofApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
		

private:
	float _timer;
	ofEasyCam _cam;

#pragma region Viewer
private:
	void setupViewer();
	void updateViewer(float delta);
	void drawViewer();
private:
	ofVec3f _symbolPos, _threeBodyPos, _armsPos;
	vSymbolMirror _viewSymbol;
	vThreeBody _viewThreeBody;
	vArms _viewArms;
#pragma endregion


#pragma region KinectMgr
private:
	kinectMgr _kinectMgr;
#pragma endregion

#pragma region armKinect
public:
	
private:
	armKinect _armKinect;
#pragma endregion



};

