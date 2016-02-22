#pragma once

#include "ofMain.h"
#include "Eye.h"
#include "EyeImageSet.h"
#include "Constants.h"


namespace eyegrid {
    
    class Grid {
    
    private:
        void init();
        
        ofFbo           eyesFbo;
        ofRectangle     rect;
        float           eyeRadius;
        
        shared_ptr<EyeImageSet> imageSet;
        vector<shared_ptr<Eye>> eyes;
        
        inline void setCols(int& c) {
            this->setup(this->rect, c, this->rows, this->imageSet);
        };
        inline void setRows(int& r) {
            this->setup(this->rect, this->cols, r, this->imageSet);
        };
        
        ofParameter<bool>   bDebugMode;
        
    public:
        Grid();
        ~Grid();
        
        void setup(const ofRectangle& rect, int cols, int rows, shared_ptr<EyeImageSet> set);
        void update();
        void draw();
        void lookAt(const ofPoint &lookAt);
        void rest();
        
        inline ofRectangle getRectangle(){
            return this->rect;
        };
        
        inline void fitEyesToRect(const ofRectangle& borderRect){
            this->setup(borderRect, this->cols, this->rows, this->imageSet);
        };
        
        ofParameterGroup    params;
        ofParameter<int>    cols;
        ofParameter<int>    rows;
    };
}