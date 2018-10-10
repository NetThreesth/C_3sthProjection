#pragma once

#include "constParameter.h"


class flowField
{
private:
	class forceUnit
	{
	public:
		forceUnit() :
			_needUpdate(false)
			, _timer(0.0f)
		{};
		void update(float delta);
		ofVec2f getForce();
		ofVec2f getBasicForce();
		void setForce(ofVec2f force);
	public:
		bool _needUpdate;
		ofVec2f _force;
		ofVec2f _basicForce;
		float _timer;
	};

public:
	void update(float delta);
	void draw(int x, int y, int width, int height);
	void setForce(int x, int y, ofVec2f force, int width, int height);
	void setForce(ofRectangle range, ofVec2f force, int width, int height);
	ofVec2f getForce(int x, int y, int width, int height);
	ofVec2f getBasicForce(int x, int y, int width, int height);

private:
	int getIndex(int x, int y, int width, int height);
	ofVec2f getPos(int x, int y, int width, int height);

private:
	array<forceUnit, cFFNum> _metaFlowMap;

	//-------------------
	//Singleton
	//-------------------
private:
	flowField()
	{

		for (int y = 0; y < cFFRowsNum; y++)
		{
			for (int x = 0; x < cFFColsNum; x++)
			{
				int index = y * cFFColsNum + x;
				
				
				float vx = x / (float)cFFColsNum + ofRandom(0.01, 0.05);
				float vy = y / (float)cFFRowsNum + ofRandom(0.01, 0.05);;
				float theta = ofMap(ofNoise(vx, vy), 0, 1, 0, TWO_PI);
				ofVec2f v(cos(theta), sin(theta));

				_metaFlowMap[index]._basicForce.set(v);
				_metaFlowMap[index]._basicForce.normalize();
				_metaFlowMap[index]._basicForce *= ofRandom(cFFBasicForceMin, cFFBasicForceMax);
			}
		}
	};
	~flowField()
	{
		flowField::destroy();
	}
	flowField(flowField const&);
	void operator=(flowField const&);

public:
	static flowField* getInstance();
	static void destroy();

private:
	static flowField *pInstance;

};