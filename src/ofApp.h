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
    void exit();
    
    void mouseMoved(int x, int y);
    void keyPressed(int key);
    
    inline void setCols(int& c) {
        eyeGrid.setup(ofGetWindowRect(), c, rows);
    };
    inline void setRows(int& r) {
        eyeGrid.setup(ofGetWindowRect(), cols, r);
    };
    
    ofVideoGrabber  grabber;
    ofxCv::ContourFinder                contourFinder;
    ofxCv::RectTrackerFollower<Glow>    tracker;
    ofxAnimatableOfPoint                lookAtPoint;
    ofFbo                               trackerFbo;
    
    EyeGrid                 eyeGrid;
    
    ofxPanel                gui;
    ofParameter<float>      threshold;
    ofParameter<int>        cols;
    ofParameter<int>        rows;
    ofParameter<bool>       bTracking;
    ofParameter<bool>       bDebugMode;
    
    bool bDrawGui;
};
