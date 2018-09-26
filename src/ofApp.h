#pragma once

#include "constParameter.h"
#include "vSymbolMirror.h"
#include "kinectMgr.h"
#include "armKinect.h"

class ofApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
		

private:
	float _timer;
	ofEasyCam _cam;


#pragma region Symbol Mirror
private:
	vSymbolMirror _viewSymbol;
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

