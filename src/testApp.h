#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxFlob.h"
#include "ofxKinect.h"
#include "ofxOpenCv.h"
#include "ofxOsc.h"
#include "Eye.h"

//#define USE_KINECT
//#define USE_BLOB_DETECTION
#define USE_OSC

class testApp : public ofBaseApp
{

public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	void windowResized(int w, int h);
    void initEyes();
    void clearEyes();
    void setupGui();
    void guiEvent(ofxUIEventArgs &e);
    void exit();
    
    void    updateEyes();
    
	float	eyeCountHorizontal, eyeCountVertical;
    float	eyeSize;
    int     maxEyesHorizontal, maxEyesVertical;
    bool    bDebugMode, bEyesInitialized;
    
    vector	<Eye*>	eyes;
    
    ofImage surfaceImg;
	ofImage whiteImg;
	ofImage pupilImg;
	ofImage shadeImg;
    ofFbo   eyesFbo;

#ifdef USE_KINECT
    ofxKinect kinect;
    
    ofxCvColorImage colorImg; // color image
	
	ofxCvGrayscaleImage grayImage; // grayscale depth image
	ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
	ofxCvGrayscaleImage grayThreshFar; // the far thresholded image
	
	ofxCvContourFinder contourFinder;
    
    int nearThreshold;
	int farThreshold;
#endif
    
#ifdef USE_OSC
    ofxOscReceiver  oscReceiver;
#endif
    

#ifdef USE_BLOB_DETECTION
	ofVideoGrabber  vidGrabber;
    Flob            flob;
    vector<ABlob*>  *blobs;
    bool            bDrawFlob;
    float           threshold;
    float           fade;
    bool            bClearBackground;
#endif
    
    // Eyes follow here
    ofVec2f     lookAtCentroid;
    
    // GUI
    ofxUICanvas *gui;
};
