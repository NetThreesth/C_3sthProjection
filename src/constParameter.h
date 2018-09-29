#pragma once

#include "ofMain.h"

//Metaball
static const int cMetaballNum = 10;
static const ofRectangle cMetaballRect(0, 0, 836, 1000);
static const int cMetaballMaxSize = min(836, 1000);

//Meta Ai Symbol
static const int cMetaAiSymbolSize = 64;
static const int cMetaAiSymbolNodeNum = cMetaAiSymbolSize * cMetaAiSymbolSize;
static const int cMetaAiPixelThreshold = 100;

//Kinect
static const int cKinectNum = 4;
static const ofRectangle cKinectMergeRect(0, 0, 1280, 960);
static const int cKinectUDPUpdateTime = 0.5f;
static const int cKMaxBlobNumEach = 5;
static const int cKTrackingMaxDist = 200 * 200;

static const int cKMergeSize = 60;

//Flow Field
static const int cFFRowsNum = 30;
static const int cFFColsNum = 40;
static const int cFFNum = cFFRowsNum * cFFColsNum;
static const float cFFFourceTime = 100.0f;

//Arm
static const int cArmKinectNum = 2;
static const int cArmBufferSize = 120;
static const float cFrameTime = 1 / 24.0f;
static const int cArmStartFrame = 219;
static const int cArmEndFrame = 2049;
static const int cArmFrameNum = cArmEndFrame - cArmStartFrame + 1;

//Mirror
static const int cMirrorImgAlphaT = 20;

//Three Body
static const float cTBMaxR = 4000.0f;
static const float cTBMinR = 50.0f;
static const int cDefaultMass = 800;
static const float cDefaultR = 763.376313;
static const int cGravity = 9;
static const int cBodyBallSize = 20;