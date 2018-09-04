#pragma once

#include "constParameter.h"
#include "metaball.h"
#include "metaAiSymbol.h"

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

private:
	metaAiSymbol _symbol;
#pragma endregion

};

