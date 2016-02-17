#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "ofxCameraSaveLoad.h"
#include "ofxAnimatableOfPoint.h"
#include "ofxAssimpModelLoader.h"
#include "EyeGrid.h"
#include "Glow.h"


class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void mouseMoved(int x, int y);
    void keyPressed(int key);
    
    ofVideoGrabber  grabber;
    ofxCv::ContourFinder                contourFinder;
    ofxCv::RectTrackerFollower<Glow>    tracker;
    ofxAnimatableOfPoint                lookAtPoint;
    ofFbo                               trackerFbo;
    
    EyeGrid                 eyeGrid;
    
    ofxPanel                gui;
    ofParameter<float>      threshold;
    ofParameter<bool>       bTracking;
    ofParameter<bool>       bDebugMode;
    
    bool bDrawGui;
};
