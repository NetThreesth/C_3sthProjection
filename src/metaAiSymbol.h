#pragma once

#include "constParameter.h"

class metaAiSymbol
{
private:
	class symbolNode {
	public:
		symbolNode();
		void init(int x, int y, ofColor c);
		void update(float delta);
		void draw();

		ofVec3f getPos();
		ofColor getColor();
	private:
		ofVec3f _pos;
		ofColor _color;
	};
	
public:
	void setup(int size, int range, float threshold);
	void update(float delta);
	void draw();

private:
	void updateLine();
	void initBaseDist(float unit);
	vector<int> getNearIndex(int x, int y, int maxNum);
	void index2xy(int index, int& x, int& y);
private:
	int _displaySize;
	float _moveRange, _threshold;
	ofPixels _text;
	
	array<bool, cMetaAiSymbolNodeNum> _symbolFlag;
	array<vector<int>, cMetaAiSymbolNodeNum> _symbolLineCheck;
	array<symbolNode, cMetaAiSymbolNodeNum> _symbolNode;
	array<float, cMetaAiSymbolSize * 2> _symbolBaseDist;

	ofVboMesh _lineMesh;
};