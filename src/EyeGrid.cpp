#include "EyeGrid.h"

//--------------------------------------------------------------
void EyeGrid::setup(float w, float h, int c, int r, float rad){
    
    this->width = w;
    this->height = h;
    this->cols = c;
    this->rows = r;
    this->eyeRadius = rad;
    
    float eyeWidth = w / MAX((int)c, 1);
    float eyeHeight = h / MAX((int)r, 1);
    this->eyeRadius = MIN(eyeWidth, eyeHeight);
    
    surfaceImg.load("surface1.png");
    whiteImg.load("white1.png");
    pupilImg.load("pupil1.png");
    shadeImg.load("shade1.png");
    
    initEyes();
}

//--------------------------------------------------------------
void EyeGrid::update(const ofPoint& lookAt){
    for (auto eye : eyes) {
        eye->lookAt(lookAt, MAX(this->width, this->height) * this->eyeRadius);
    }
}

//--------------------------------------------------------------
void EyeGrid::draw(const bool& debugMode){
    ofSetColor(ofColor::white);
    bool bDebugMode = debugMode;
    for (auto eye : eyes) {
        eye->draw(&bDebugMode);
    }
    
    eyesFbo.draw(0, 0, this->width, this->height);
}

//--------------------------------------------------------------
void EyeGrid::initEyes(){
    eyesFbo.allocate(this->width, this->height);
    eyesFbo.begin();
    ofClear(0,0,0,0);
    ofSetColor(ofColor::white);
    eyesFbo.end();
    
    for (int i = 0; i<(int)this->cols; i ++){
        for (int j = 0; j<(int)this->rows; j++){
            
            shared_ptr<Eye> eye(new Eye);
            // Set pixel data before eye setup
            eye->surfaceImg.setFromPixels(surfaceImg.getPixels());
            eye->whiteImg.setFromPixels(whiteImg.getPixels());
            eye->pupilImg.setFromPixels(pupilImg.getPixels());
            eye->shadeImg.setFromPixels(shadeImg.getPixels());
            eye->setup(ofVec2f(i*this->eyeRadius, j*this->eyeRadius),
                       this->eyeRadius,
                       this->eyeRadius);
            eyes.push_back(eye);
            
            ofLog(OF_LOG_VERBOSE, "Added eye j:" + ofToString(j) + ", i:" + ofToString(i));
        }
    }
}

//--------------------------------------------------------------
void EyeGrid::rest() {
    for (auto eye : eyes) {
        eye->rest();
    }
}

