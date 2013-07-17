#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxFlob.h"
#include "ofxKinect.h"
#include "ofxOpenCv.h"
#include "Eye.h"

//#define USE_KINECT

class testApp : public ofBaseApp
{

public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	void windowResized(int w, int h);
    void mousePressed(int x, int y);
    void initEyes();
    void clearEyes();
    void setupGui();
    void guiEvent(ofxUIEventArgs &e);
    void exit();

	float	eyeCountHorizontal;
	float	eyeCountVertical;
    bool    bDebugMode, bEyesInitialized;
    float	minSize;

#ifdef USE_KINECT
    ofxKinect kinect;
    
    ofxCvColorImage colorImg;
	
	ofxCvGrayscaleImage grayImage; // grayscale depth image
	ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
	ofxCvGrayscaleImage grayThreshFar; // the far thresholded image
	
	ofxCvContourFinder contourFinder;
    
    int nearThreshold;
	int farThreshold;
#endif

	vector	<Eye*>	eyes;
    
    ofImage surfaceImg;
	ofImage whiteImg;
	ofImage pupilImg;
	ofImage shadeImg;

	ofVideoGrabber vidGrabber;
    Flob flob;
    vector<ABlob*> *blobs;
    bool bDrawFlob;
    float threshold;
    float fade;
    bool bClearBackground;
    
    ofVec2f lookAtCentroid;
    bool bFollowMouse;
    
    ofxUICanvas *gui;
};
