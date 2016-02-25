#pragma once

#include "ofMain.h"
#include "Constants.h"


namespace tracker {
    
    class BaseTracker {
        
    protected:
        
        ofFbo trackerFbo;
        
    public:
        
        BaseTracker(){
            params.setName("Base Tracker");
            params.add(drawPos.set("Output Draw Position",
                                   ofVec2f(ofGetWidth() - (GRABBER_WIDTH + 20), 20),
                                   ofVec2f::zero(),
                                   ofVec2f(ofGetWidth() - (GRABBER_WIDTH + 20), ofGetHeight() - (GRABBER_HEIGHT + 20))));
            params.add(enabled.set("Enabled", false));
            trackerFbo.allocate(GRABBER_WIDTH, GRABBER_HEIGHT);
        }
        
        void begin(){
            trackerFbo.begin();
            ofClear(0, 0, 0, 0);
            ofSetColor(ofColor::white);
        }
        void end(){
            trackerFbo.end();
        }
        
        virtual void track(const ofPixels& pixels){}
        virtual void draw(){
            if (enabled) {
                trackerFbo.draw(drawPos.get(), GRABBER_WIDTH, GRABBER_HEIGHT);
            }
        }
        
        ofParameter<ofVec2f>    drawPos;
        ofParameter<bool>       enabled;
        ofParameterGroup        params;
        
    };
};