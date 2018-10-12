#pragma once

#include "constParameter.h"

class multiCam
{
#pragma region CLASS camUnit
private:
	class camUnit
	{
	public:
		camUnit();
		void update(float delta);
		void draw(int x, int y, int width, int height);
		void begin();
		void end();
	public:
		ofEasyCam _cam;

	private:
		ofFbo _canvas;
		bool _setBegin;
		
	};
#pragma endregion
	
public:
	multiCam();
	void setup(int x, int y, int width, int height);
	void update(float delta);
	void updateParent(ofNode& p);

	void draw();
	void drawCam();

	int getCamNum();

	void begin(int i);
	void end();

	//GUI
	void onCamDistChange(int& dist);

private:
	bool _isSetup;
	int _drawCamIdx;
	float _unitW, _unitH;
	ofRectangle _displayRect;
	ofNode _parent;
	ofImage _mask;
	array<camUnit, cMCCamNum> _camList;
};