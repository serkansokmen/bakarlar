#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "BaseTracker.h"
#include "Constants.h"


namespace tracker {
    class ColorTracker : public BaseTracker {
        
    private:
        ofxCv::ContourFinder        contourFinder;
        ofxCv::TrackingColorMode    trackingColorMode;
        ofImage colorImage;
        
        void setMinAreaRadius(float& val);
        void setMaxAreaRadius(float& val);
        void setThreshold(float& val);
        void setTrackingColor(ofColor& color);
        
    public:
        ofParameter<ofColor>    targetColor;
        ofParameter<float>      minAreaRadius, maxAreaRadius, threshold;
        ofParameter<int>        blur;
        
        ColorTracker();
        ~ColorTracker();
        
        void track(const ofPixels& pixels);
        void draw();
        
        inline const vector<cv::Rect>& getBoundingRects() const {
            return contourFinder.getBoundingRects();
        }
    };
};
