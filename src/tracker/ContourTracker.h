#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxKinect.h"
#include "ofxGui.h"
#include "Glow.h"
#include "Constants.h"


using namespace cv;


namespace tracker {
    class ContourTracker {
        
    private:
        ofVideoGrabber                      grabber;
        ofxKinect                           kinect;
        ofxCv::ContourFinder                contourFinder;
        ofxCv::RectTrackerFollower<Glow>    tracker;
        ofFbo                               trackerFbo;
        
        ofImage grayImage;
        ofImage grayThreshNear;
        ofImage grayThreshFar;
        ofImage grayPreprocImage;
        cv::Mat src;
        ofxCv::TrackingColorMode    trackingColorMode;
        
        void toggleGrabber(bool& yes);
        void toggleKinect(bool& yes);
        void updateGrabber();
        void updateKinect();
        
    public:
        ofParameter<ofColor>    targetColor;
        ofParameter<float>      minAreaRadius, maxAreaRadius, threshold;
        ofParameter<float>      nearThreshold, farThreshold;
        ofParameter<int>        persistence, maxDistance, blur;
        ofParameter<bool>       bUseGrabber;
        ofParameter<bool>       bUseKinect;
        ofParameter<bool>       bDraw;
        ofParameterGroup        params;
        
        ContourTracker();
        ~ContourTracker();
        
        void setup();
        void update();
        inline void draw(){
            this->draw(0, 0, 1.0f);
        };
        void draw(float x, float y, float scale);
        void drawPointCloud();
        
        inline bool isTracking() {
            return bUseGrabber == true || bUseKinect == true;
        }
        inline const vector<cv::Rect>& getBoundingRects() const {
            return contourFinder.getBoundingRects();
        }
        inline const ofPoint getAverage() const {
            
        }
        inline void setTrackingColor(ofColor& c) {
            contourFinder.setTargetColor(c, trackingColorMode);
        }
        inline ofColor getColorAt(int x, int y) {
            return ofColor(grabber.getPixels().getColor(x, y));
        }
    };
};
