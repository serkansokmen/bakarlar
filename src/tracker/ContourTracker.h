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
        
        void toggleGrabber(bool& yes);
        void toggleKinect(bool& yes);
        void updateGrabber();
        void updateKinect();
        
    public:
        ofParameter<float>      minAreaRadius, maxAreaRadius, threshold;
        ofParameter<float>      nearThreshold, farThreshold;
        ofParameter<int>        persistence, maxDistance;
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
        
        inline const bool& isTracking(){
            return (bUseGrabber || bUseKinect);
        }
        inline const vector<cv::Rect>& getBoundingRects() const {
            return contourFinder.getBoundingRects();
        }
        inline const ofPoint getAverage() const {
            
        }
    };
};
