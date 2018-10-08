#pragma once

#include "ofMain.h"




static const int cImg2MeshAlpahT = 20;

//Metaball
static const int cMetaballNum = 10;
static const int cMetaballBasicFlowNum = 4;
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

//Kinect Arm
static const int cArmFilterY = -70;
static const int cArmKinectNum = 2;
static const int cArmBufferSize = 120;
static const int cArmPlaySpeedNum = 6;
static const float cArmPlayBaseFPS = 24.0f;
static const float cArmPlaySpeedEach = cArmBufferSize / (float)cArmPlaySpeedNum;
static const float cFrameTime = 1 / cArmPlayBaseFPS;
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

//Particle
static const ofVec3f cPCenter(0, -100, 0);
static float cPBaseRadius = 300.0f;
static float cPFadeDuration = 3.0f;
static float cPDeadHigh = 3000;
static float cPSphereSize = 4.0f;
static float cPEmitterMinT = 0.5f;
static float cPEmitterMaxT = 1.5f;

//Multi-Cam
static const int cMCCamNum = 6;
static const int cMCCamCanvasWidth = 1920;
static const int cMCCamCanvasHeight = 1080;
static const int cMCCamDist = 100;
static const int cMCTargetDepth = 300;

//------------------------------------------
//Views
//View Camera
static const ofRectangle cViewCamSize(0, 0, 1920, 1080);
static const float cViewFadeT = 3.0f;

//Arms View
static const ofVec3f cArmsShiftPos(0, 0, 250);
static const int cArmsSpaceSize = 500;
static const int cArmsCeilingAlpha = 255;
static const int cArmsMirrorAlpha = 255;
static const int cArmsMirrorNum = 2;
static const float cArmsSpaceRotateV = 10.0f;
static const ofVec3f cAmrsCeilTopPos(0, cArmsSpaceSize, 0);

//Three Body
static const ofVec3f cThreeBodyPos(0, 3000, 0);

//Symbol
static const ofVec3f cSymbolPos(0, 5000, 0);
static const ofVec3f cSymbolViewHeight(0, 4800, 0);
static const float cSymbolDisplayHeight = cSymbolViewHeight.y * 0.8f;
static const float cSymbolDisplayTime = 10.0f;

//Partical
static const ofVec3f cParticlePos(0, -100, 0);

//Viewer Camera
static const ofVec3f cViewCamDefaultPos(0, 0, 400);
static const ofVec3f cViewCamDefaultTarget(0, 0, -1);
static const float cViewArmsT = 3.0f;
static const float cViewThreeBoydT = 5.0f;