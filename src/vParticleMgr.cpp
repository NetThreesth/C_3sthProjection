#include "vParticleMgr.h"

//-------------------------------
vParticleMgr::vParticleMgr()
	:_isStart(false)
{
	_particleList.clear();
	_animAlpha.reset(0.0f);
	_animAlpha.setDuration(cPFadeDuration);
}

//-------------------------------
void vParticleMgr::update(float delta)
{
	if (!_isStart)
	{
		return;
	}

	if (_isEmitted)
	{
		_timer -= delta;
		if (_timer <= 0.0f)
		{
			emitter(1 + rand() % 10);
			if (_particleWaitSize <= 0)
			{
				_isEmitted = false;
			}
			else
			{
				_timer = ofRandom(cPEmitterMinT, cPEmitterMaxT);
			}
		}
	}
	
	_animAlpha.update(delta);
	if (_animAlpha.hasFinishedAnimating() && _animAlpha.getPercentDone() == 1.0)
	{
		if (_animAlpha.getCurrentValue() == 0.0f)
		{
			_isStart = false;
			_particleList.clear();
		}
	}
	updateParticle(delta);
	
}

//-------------------------------
void vParticleMgr::draw(ofVec3f pos)
{

	ofPushStyle();
	ofSetColor(255, _animAlpha.getCurrentValue());
	{
		ofPushMatrix();
		ofTranslate(pos);
		{
			for (auto& iter : _particleList)
			{
				ofDrawSphere(iter.getDrawPos(), cPSphereSize);
			}
		}
		ofPopMatrix();
	}
	ofPopStyle();
}

//-------------------------------
void vParticleMgr::start(int num)
{
	_isStart = _isEmitted = true;
	_animAlpha.animateTo(255);
	_timer = ofRandom(cPEmitterMinT, cPEmitterMaxT);
	_particleWaitSize = num;
}

//-------------------------------
void vParticleMgr::stop()
{
	_animAlpha.animateTo(0);
	_particleList.clear();
}


//-------------------------------
int vParticleMgr::getParticleNum()
{
	return _particleList.size();
}

//-------------------------------
void vParticleMgr::emitter(int num)
{
	auto p = MIN(_particleWaitSize, num);
	_particleWaitSize -= p;
	for (int i = 0; i < p; i++)
	{	
		_particleList.push_back(particle());
	}
}

//-------------------------------
void vParticleMgr::updateParticle(float delta)
{
	for (auto& iter : _particleList)
	{
		iter.update(delta, cPDeadHigh);
	}

	auto iter = remove_if(_particleList.begin(), _particleList.end(),
		[&](particle& p) -> bool{
			return p.checkLife();
		}
	);
	_particleList.erase(iter, _particleList.end());
}
