#pragma once

#include "constParameter.h"
#include "metaball.h"
#include "metaAiSymbol.h"
#include "kinectMgr.h"
#include "flowField.h"
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

	metaball _mb;

#pragma region metaAiText
public:
	void initSymbol();
private:
	int _symbolIndex;
	vector<symbol> _symbolList;
	metaAiSymbolDisplay _symbolDisplay;
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

