#pragma once

#include "ofMain.h"
#include "Eye.h"
#include "EyeImageSet.h"


namespace eye {
    
    class Grid {
        
        void init();
        
        ofFbo   eyesFbo;
        
        float   width, height;
        float   eyeRadius;
        int     cols, rows;
        
        shared_ptr<ImageSet>    imageSet;
        
        vector<shared_ptr<Eye>> eyes;
        
    public:
        ~Grid();
        
        void setup(const ofRectangle& rect, int cols, int rows, shared_ptr<ImageSet> set);
        void update();
        void draw(const bool& debugMode);
        void lookAt(const ofPoint &lookAt);
        void rest();
        
        inline float getWidth(){
            return this->width;
        };
        inline float getHeight(){
            return this->height;
        };
    };
}