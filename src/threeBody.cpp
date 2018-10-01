#include "threeBody.h"

//-----------------------------
void threeBody::setup()
{

	_bodyA._pos.set(cDefaultR*cos(0), 0);
	_bodyB._pos.set(cDefaultR*cos(2 * PI / 3), cDefaultR*sin(2 * PI / 3));
	_bodyC._pos.set(cDefaultR*cos(4 * PI / 3), cDefaultR*sin(4 * PI / 3));

	//    cout << "posA: " << posA << " ; " << "posB: " << posB << " ; " << "posC: " << posC << endl;

	float distAB = _bodyA.dist(_bodyB);
	float distBC = _bodyB.dist(_bodyC);
	float distAC = _bodyA.dist(_bodyC);

	//set initial vel
	float forceAB = cGravity*_bodyA._mass*_bodyB._mass / (distAB*distAB);

	float _VEL = sqrt(forceAB*cos(PI / 6) * 2 * cDefaultR / cDefaultMass);

	_bodyA._vel.set(-_VEL*sin(0), _VEL*cos(0), -1);
	_bodyB._vel.set(-_VEL*sin(2 * PI / 3), _VEL*cos(2 * PI / 3), 0);
	_bodyC._vel.set(-_VEL*sin(4 * PI / 3), _VEL*cos(4 * PI / 3), 1);

	_bodyA._color.set(255, 0, 0, 100);
	_bodyB._color.set(0, 255, 0, 100);
	_bodyC._color.set(0, 0, 255, 100);
	_mesh.setMode(OF_PRIMITIVE_LINE_LOOP);

	_timer = cTBUpdateTime;
	_meshTimer = cTBAddMeshTime;
	_isSetup = true;
}

//-----------------------------
void threeBody::update(float delta)
{
	if (!_isSetup)
	{
		return;
	}

	_timer -= delta;
	if (_timer <= 0.0f)
	{
		oneStep();
		_timer = cTBUpdateTime;
	}

	_meshTimer -= delta;
	if (_meshTimer <= 0.0f)
	{
		updateMesh();
		_meshTimer = cTBAddMeshTime;
	}
}

//-----------------------------
void threeBody::draw()
{
	if (!_isSetup)
	{
		return;
	}

	ofPushStyle();
	ofSetColor(255);
	{
		_bodyA.draw();
		_bodyB.draw();
		_bodyC.draw();
		_mesh.draw();
	}
	ofPopStyle();
}

//-----------------------------
void threeBody::reset()
{
	_mesh.clear();
}

//-----------------------------
void threeBody::oneStep()
{
	int totalMass = _bodyA._mass + _bodyB._mass + _bodyC._mass;
	float distAB = (_bodyA.dist(_bodyB));
	float distAC = (_bodyA.dist(_bodyC));
	float distBC = (_bodyB.dist(_bodyC));

	//attracation
	ofVec3f attA = getAttFouce(_bodyA, totalMass);
	ofVec3f attB = getAttFouce(_bodyB, totalMass);
	ofVec3f attC = getAttFouce(_bodyC, totalMass);

	//flee
	ofVec3f fleeA, fleeB, fleeC;
	fleeBetween(_bodyA, _bodyB, distAB, fleeA, fleeB);
	fleeBetween(_bodyB, _bodyC, distBC, fleeB, fleeC);
	fleeBetween(_bodyA, _bodyC, distAC, fleeA, fleeC);

	//net force
	float fAB = cGravity * _bodyA._mass * _bodyB._mass / (distAB*distAB);
	float fAC = cGravity * _bodyA._mass * _bodyC._mass / (distAC*distAC);
	float fBC = cGravity * _bodyB._mass * _bodyC._mass / (distBC*distBC);

	ofVec3f accA = getAccForce(_bodyA, _bodyB._pos - _bodyA._pos, _bodyC._pos - _bodyA._pos, fAB, fAC, distAB, distAC);
	ofVec3f accB = getAccForce(_bodyB, _bodyA._pos - _bodyB._pos, _bodyC._pos - _bodyB._pos, fAB, fBC, distAB, distBC);
	ofVec3f accC = getAccForce(_bodyC, _bodyA._pos - _bodyC._pos, _bodyB._pos - _bodyC._pos, fAC, fBC, distAC, distBC);

	_bodyA.update(accA, attA, fleeA);
	_bodyB.update(accB, attB, fleeB);
	_bodyC.update(accC, attC, fleeC);
}

//-----------------------------
void threeBody::updateMesh()
{
	_mesh.addVertex(_bodyA._pos);
	_mesh.addColor(_bodyA._color);

	_mesh.addVertex(_bodyB._pos);
	_mesh.addColor(_bodyB._color);

	_mesh.addVertex(_bodyC._pos);
	_mesh.addColor(_bodyC._color);

	if (_mesh.getNumVertices() > cTBMeshMax)
	{
		_mesh.removeVertex(0);
		_mesh.removeVertex(0);
		_mesh.removeVertex(0);

		_mesh.removeColor(0);
		_mesh.removeColor(0);
		_mesh.removeColor(0);
	}
}

//-----------------------------
ofVec3f threeBody::getAttFouce(stBody & body, int totalMass)
{
	ofVec3f att(0);
	float dist = body._pos.distance(ofVec3f(0));

	if (dist > cTBMaxR)
	{
		float forceM = cGravity * body._mass * totalMass / (dist * dist);
		ofVec3f force = (body._pos * -1) / dist * forceM;
		att = (force / body._mass) * 2.0;
	}
	return att;
}

//-----------------------------
void threeBody::fleeBetween(stBody & b1, stBody & b2, float dist, ofVec3f & flee1, ofVec3f & flee2)
{
	if (dist < cTBMinR)
	{
		float distPow = dist * dist;
		ofVec3f mid = (b1._pos + b2._pos) / 2.0;
		float f1 = cGravity * b1._mass * (b1._mass + b2._mass) / distPow;
		float f2 = cGravity * b2._mass * (b1._mass + b2._mass) / distPow;

		ofVec3f force1 = f1 * (b1._pos - mid) / dist;
		ofVec3f force2 = f2 * (b2._pos - mid) / dist;

		flee1 += force1 / b1._mass;
		flee2 += force2 / b2._mass;
	}
}

//-----------------------------
ofVec3f threeBody::getAccForce(stBody & b, ofVec3f diff1, ofVec3f diff2, float f1, float f2, float d1, float d2)
{
	ofVec3f acc = f1 * diff1 / d1 + f2 * diff2 / d2;
	acc /= b._mass;
	return acc;
}
