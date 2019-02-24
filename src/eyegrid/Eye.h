#pragma once

#include "ofMain.h"
#include "ofxAnimatableOfPoint.h"
#include "EyeImageSet.h"
#include "Constants.h"


namespace eyegrid {
    
    class Eye {
        
    private:
        
        shared_ptr<ofImage> surfaceImg;
        shared_ptr<ofImage> whiteImg;
        shared_ptr<ofImage> pupilImg;
        shared_ptr<ofImage> shadeImg;
        
        float eyeWidth;
        float eyeHeight;
        float eyeRadius;
        
        ofFbo fbo;
        
    public:
        
        void setup(const ofVec2f& pos, float w, float h);
        void update();
        void draw(const bool& debugMode);
        
//        ofxAnimatableOfPoint  pupilPos;
        ofVec2f               lookAt;
        ofVec2f               restPos;
        
        inline void setImageLayer(shared_ptr<ofImage> img, const string& layerName) {
            if (layerName == "surface") {
                this->surfaceImg = img;
            }
            if (layerName == "white") {
                this->whiteImg = img;
            }
            if (layerName == "pupil") {
                this->pupilImg = img;
            }
            if (layerName == "shade") {
                this->shadeImg = img;
            }
        };
    };
}
