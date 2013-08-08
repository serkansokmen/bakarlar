#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxFlob.h"
#include "ofxKinect.h"
#include "ofxOpenCv.h"
#include "ofxOsc.h"
#include "ofxTweener.h"
#include "Eye.h"

//#define USE_KINECT
//#define USE_BLOB_DETECTION
//#define USE_OSC
#define USE_MOUSE

#define MAX_EYES_HORIZONTAL 32
#define MAX_EYES_VERTICAL   20

class testApp : public ofBaseApp
{

public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
#ifdef USE_MOUSE
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
#endif
	void windowResized(int w, int h);
    void initEyes();
    void clearEyes();
    void setupGui();
    void guiEvent(ofxUIEventArgs &e);
    void exit();
    
    void    updateEyes();
    
	float	eyeCountHorizontal, eyeCountVertical;
    float	eyeSize;
    bool    bDebugMode, bEyesInitialized;
    
    vector	<Eye*>	eyes;
    // Eyes follow here
    ofVec2f lookAtCentroid;
    ofVec2f debugCirclePos;
    
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
    
    // GUI
    ofxUICanvas *gui;
};
