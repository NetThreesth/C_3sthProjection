#pragma once

#include "particle.h"
#include "ofxAnimatableFloat.h"

class vParticleMgr
{
public:
	vParticleMgr();
	
	void update(float delta);
	void draw(ofVec3f pos);

	void start(int num);
	void stop();

private:
	void emitter(int num);
	int getParticleNum();
	void updateParticle(float delta);

private:
	bool _isStart, _isEmitted;
	int _particleWaitSize;
	float _timer;
	ofxAnimatableFloat _animAlpha;
	list<particle> _particleList;
};