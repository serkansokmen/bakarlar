#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "BaseTracker.h"
#include "Constants.h"


namespace tracker {
    class ColorTracker : public BaseTracker {
        
    private:
        ofxCv::ContourFinder contourFinder;
        
        ofImage colorImage;
        ofImage grayImage;
        ofImage grayThreshNear;
        ofImage grayThreshFar;
        ofImage grayPreprocImage;
        cv::Mat src;
        ofxCv::TrackingColorMode    trackingColorMode;
        
    public:
        ofParameter<ofColor>    targetColor;
        ofParameter<float>      minAreaNorm, threshold;
        ofParameter<int>        blur;
        
        ColorTracker();
        ~ColorTracker();
        
        void track(const ofPixels& pixels);
        void draw();
        
        inline const vector<cv::Rect>& getBoundingRects() const {
            return contourFinder.getBoundingRects();
        }
//        inline const ofPoint getAverage() const {
//            
//        }
        inline void setTrackingColor(ofColor& c) {
            contourFinder.setTargetColor(c, trackingColorMode);
        }
    };
};
