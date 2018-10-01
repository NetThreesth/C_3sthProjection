#pragma once

#include "constParameter.h"
#include "armKinect.h"

class vArms
{
public:
	vArms()
		:_isSetup(false)
		,_isStart(false)
		,_displayStage(false)
	{}

	void setup();
	void update(float delta);
	void draw(ofVec3f pos);
	
	void start();
	void stop();

	void setStage(bool isDisplay);

private:
	bool _isSetup, _isStart;
	

#pragma region armKinect
private:
	ofVec3f _shiftPos;
	armKinect _arms;
#pragma endregion

#pragma region stage
private:
	bool initStage();
	void updateStage(float delta);
	void drawStage();
	bool loadCeiling();
	bool loadMirror(ofVboMesh& mesh, string name);

private:
	bool _displayStage;
	float _rotD, _rotV;
	ofColor _spaceColor;
	ofVboMesh _ceiling;
	ofVboMesh _mirrors[cArmsMirrorNum];
	ofSpherePrimitive _space;
#pragma endregion



};