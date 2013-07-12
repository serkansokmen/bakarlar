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
	void keyPressed  (int key);
	void windowResized(int w, int h);

	void exit();
    void setupGui();
    void guiEvent(ofxUIEventArgs &e);
    void initEyes();

	float	eyeCountHorizontal;
	float	eyeCountVertical;
    bool    bDebugMode, bEyesInitialized;
    float	minSize;

	vector	<Eye*>	eyes;

	ofVideoGrabber			vidGrabber;
    Flob flob;
    bool bDrawFlob;

	vector<ABlob*> *blobs;

    ofxUICanvas *gui;

    float threshold;
    float fade;
    bool bClearBackground;
};
