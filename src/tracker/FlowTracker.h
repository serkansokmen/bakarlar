#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "BaseTracker.h"
#include "Constants.h"


namespace tracker {
    
    class FlowTracker : public BaseTracker {
        
        ofxCv::FlowFarneback fb;
        ofxCv::FlowPyrLK lk;
        
        ofxCv::Flow* curFlow;
        
    public:
        
        FlowTracker();
        ~FlowTracker();
        
        void track(const ofPixels& pixels);
        void draw();
        
        ofParameter<float>  fbPyrScale, lkQualityLevel, fbPolySigma;
        ofParameter<int>    fbLevels, lkWinSize, fbIterations, fbPolyN,
                            fbWinSize, lkMaxLevel, lkMaxFeatures, lkMinDistance;
        ofParameter<bool>   fbUseGaussian, usefb;
    };
};
