#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxKinect.h"
#include "ofxCameraSaveLoad.h"
#include "ColorTracker.h"
#include "FlowTracker.h"
#include "Grid.h"


class ofApp : public ofBaseApp{
    
public:
    void setup();
    void setupParams();
    void update();
    void draw();
    void drawPointCloud();
    void exit();
    
    void mousePressed(int x, int y, int button);
    void mouseMoved(int x, int y);
    void keyPressed(int key);
    void windowResized(int w, int h);
    
    void toggleGrabber(bool& yes);
    void toggleKinect(bool& yes);
    
    inline bool isTracking() {
        return bUseGrabber == true || bUseKinect == true;
    }
    
    ofVideoGrabber          grabber;
    ofxKinect               kinect;
    tracker::ColorTracker   colorTracker;
    tracker::FlowTracker    flowTracker;
    eyegrid::Grid           eyeGrid;
    unique_ptr<ofPoint>     lookAt;
    
    ofxPanel                gui;
    ofParameter<bool>       bUseGrabber;
    ofParameter<bool>       bUseKinect;
    ofRectangle             gridRect;
    
    bool bDrawGui;
};
