#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "ofxCameraSaveLoad.h"
#include "ofxAssimpModelLoader.h"
#include "EyeGrid.h"
#include "Glow.h"
#include "Constants.h"


class ofApp : public ofBaseApp{
    
public:
    void setup();
    void setupParams();
    void update();
    void draw();
    void exit();
    
    void mouseMoved(int x, int y);
    void keyPressed(int key);
    void windowResized(int w, int h);
    
    inline void setCols(int& c) {
        eyeGrid.setup(this->gridRect, c, this->rows, this->eyeImageSet);
    };
    inline void setRows(int& r) {
        eyeGrid.setup(this->gridRect, this->cols, r, this->eyeImageSet);
    };
    inline void fitGridRect(){
        float w = ofGetWidth();
        float h = ofGetHeight();
        float x = 0;
        float y = 0;
        gridRect.set(x, y, w, h);
        eyeGrid.setup(this->gridRect, this->cols, this->rows, this->eyeImageSet);
    };
    
    ofVideoGrabber                      grabber;
    ofxCv::ContourFinder                contourFinder;
    ofxCv::RectTrackerFollower<Glow>    tracker;
    ofFbo                               trackerFbo;
    
    eye::Grid                           eyeGrid;
    shared_ptr<eye::ImageSet>           eyeImageSet;
    
    ofxPanel                gui;
    ofParameterGroup        gridParams, trackerParams;
    ofRectangle             gridRect;
    
    ofParameter<float>      minAreaRadius, maxAreaRadius, threshold;
    ofParameter<int>        persistence, maxDistance;
    ofParameter<int>        cols;
    ofParameter<int>        rows;
    ofParameter<bool>       bTracking;
    ofParameter<bool>       bDebugMode;
    
    bool bDrawGui;
};
