#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxFlob.h"
#include "Eye.h"


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
    void setupGui();
    void guiEvent(ofxUIEventArgs &e);
    void exit();

	float	eyeCountHorizontal;
	float	eyeCountVertical;
    bool    bDebugMode, bEyesInitialized;
    float	minSize;

	vector	<Eye*>	eyes;
    
    ofImage surfaceImg;
	ofImage whiteImg;
	ofImage pupilImg;
	ofImage shadeImg;

	ofVideoGrabber vidGrabber;
    Flob flob;
    bool bDrawFlob;

	vector<ABlob*> *blobs;

    ofxUICanvas *gui;

    float threshold;
    float fade;
    bool bClearBackground;
};
