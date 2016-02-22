#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxCameraSaveLoad.h"
#include "ContourTracker.h"
#include "Grid.h"


class ofApp : public ofBaseApp{
    
public:
    void setup();
    void setupParams();
    void update();
    void draw();
    void drawPointCloud();
    void exit();
    
    void mouseMoved(int x, int y);
    void keyPressed(int key);
    void windowResized(int w, int h);
    
    tracker::ContourTracker             tracker;
    eyegrid::Grid                       eyeGrid;
    shared_ptr<eyegrid::EyeImageSet>    eyeImageSet;
    
    ofxPanel                gui;
    ofRectangle             gridRect;
    
    bool bDrawGui;
};
