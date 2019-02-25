#pragma once

#include "ofMain.h"
#include "Eye.h"
#include "EyeImageSet.h"
#include "Constants.h"


namespace eyegrid {
    
    class Grid {
    
    private:
        void setupEyes();
        
        ofFbo           eyesFbo;
        ofRectangle     rect;
        float           eyeRadius;
        
        shared_ptr<EyeImageSet> imageSet;
        vector<shared_ptr<Eye>> eyes;
        
        inline void setCols(int& c) {
            this->setup(this->rect, c, this->rows);
        };
        inline void setRows(int& r) {
            this->setup(this->rect, this->cols, r);
        };
        
        bool    isLoading;
        
    public:
        Grid();
        ~Grid();
        
        void setup(const ofRectangle& rect, int cols, int rows);
        void update(const vector<ofVec2f>& poseVecs);
        void draw();
        
        inline ofRectangle getRectangle(){
            return this->rect;
        };
        
        inline void fitEyesToRect(const ofRectangle& borderRect){
            this->setup(borderRect, this->cols, this->rows);
        };
        
        ofParameterGroup    params;
        ofParameter<int>    cols;
        ofParameter<int>    rows;
        ofParameter<bool>   bDebugMode;
    };
}
