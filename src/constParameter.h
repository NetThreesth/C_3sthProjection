#pragma once

#include "ofMain.h"

static const int cImg2MeshAlpahT = 20;

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
static const int cArmFilterY = -70;
static const int cArmKinectNum = 2;
static const int cArmBufferSize = 120;
static const float cFrameTime = 1 / 24.0f;
static const int cArmStartFrame = 219;
static const int cArmEndFrame = 2049;
static const int cArmFrameNum = cArmEndFrame - cArmStartFrame + 1;



//Three Body
static const float cTBUpdateTime = 1.0f / 24.0f; // 24fps
static const float cTBAddMeshTime = 0.1f;
static const float cTBMaxR = 4000.0f;
static const float cTBMinR = 50.0f;
static const int cDefaultMass = 800;
static const float cDefaultR = 763.376313;
static const int cGravity = 9;
static const int cBodyBallSize = 20;
static const int cTBMeshMax = 3 * 1000;

//Arms
static const ofVec3f cArmsShiftPos(0, 0, 250);
static const int cArmsSpaceSize = 500;
static const int cArmsCeilingAlpha = 255;
static const int cArmsMirrorAlpha = 255;
static const int cArmsMirrorNum = 2;
static const float cArmsSpaceRotateV = 10.0f;