#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "ofxCameraSaveLoad.h"
#include "ofxAssimpModelLoader.h"
#include "EyeGrid.h"
#include "Glow.h"


#define GRABBER_WIDTH   640
#define GRABBER_HEIGHT  480



class ofApp : public ofBaseApp{
    
public:
    void setup();
    void setupParams();
    void update();
    void draw();
    void exit();
    
    void mouseMoved(int x, int y);
    void keyPressed(int key);
    
    inline void setCols(int& c) {
        eyeGrid.setup(this->eyeGridRect, c, this->rows, this->eyeImageSet);
    };
    inline void setRows(int& r) {
        eyeGrid.setup(this->eyeGridRect, this->cols, r, this->eyeImageSet);
    };
    
    ofVideoGrabber                      grabber;
    ofxCv::ContourFinder                contourFinder;
    ofxCv::RectTrackerFollower<Glow>    tracker;
    ofFbo                               trackerFbo;
    
    eye::Grid                           eyeGrid;
    shared_ptr<eye::ImageSet>           eyeImageSet;
    ofRectangle                         eyeGridRect;
    
    ofxPanel                gui;
    ofParameterGroup        gridParams, trackerParams;
    
    ofParameter<float>      minAreaRadius, maxAreaRadius, threshold;
    ofParameter<int>        persistence, maxDistance;
    ofParameter<int>        cols;
    ofParameter<int>        rows;
    ofParameter<bool>       bTracking;
    ofParameter<bool>       bDebugMode;
    
    bool bDrawGui;
};
