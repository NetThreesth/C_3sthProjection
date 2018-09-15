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
		void setForce(ofVec2f force);
	public:
		bool _needUpdate;
		ofVec2f _force;
		float _timer;
	};

public:
	void update(float delta);
	void draw(int x, int y, int width, int height);
	void setForce(int x, int y, ofVec2f force, int width, int height);

private:
	int getIndex(int x, int y, int width, int height);
	ofVec2f getPos(int x, int y, int width, int height);

private:
	array<forceUnit, cFFNum> _flowMap;

//-------------------
//Singleton
//-------------------
private:
	flowField()
	{
		for (auto& iter : _flowMap)
		{
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