#pragma once

#include "ofMain.h"
#include "Eye.h"


class EyeGrid {
    
    void setupEyes();
    
    ofImage surfaceImg;
    ofImage whiteImg;
    ofImage pupilImg;
    ofImage shadeImg;
    ofFbo   eyesFbo;
    
    float width, height;
    float eyeRadius;
    int   cols, rows;
    
    vector<shared_ptr<Eye> >       eyes;
    
public:
    inline ~EyeGrid(){
        eyes.clear();
    };
    
    void setup(const ofRectangle& rect, int cols, int rows);
    void setupEyes(int cols, int rows);
    void update(const ofPoint &lookAt);
    void draw(const bool& debugMode);
    void rest();
    
    inline float getWidth(){
        return this->width;
    };
    inline float getHeight(){
        return this->height;
    };
};