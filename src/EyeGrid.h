#pragma once

#include "ofMain.h"
#include "Eye.h"


class EyeGrid {
    
    void initEyes();
    
    ofImage surfaceImg;
    ofImage whiteImg;
    ofImage pupilImg;
    ofImage shadeImg;
    ofFbo   eyesFbo;
    
    int cols, rows;
    float width, height;
    float eyeRadius;
    
    vector<shared_ptr<Eye> >       eyes;
    
public:
    inline ~EyeGrid(){
        eyes.clear();
    };
    
    void setup(float gridWidth, float gridHeight, int cols, int rows, float eyeRadius);
    void update(const ofPoint &lookAt);
    void draw(const bool& debugMode);
    void rest();
};