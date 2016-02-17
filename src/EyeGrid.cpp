#include "EyeGrid.h"

//--------------------------------------------------------------
void EyeGrid::setup(const ofRectangle& rect, int c, int r){
    
    float edgeLength = MIN(rect.getWidth(), rect.getHeight());
    
    this->width = rect.getWidth();
    this->height = rect.getHeight();
    this->cols = MAX((int)c, 1);
    this->rows = MAX((int)r, 1);
    
    float eyeWidth = this->width / this->cols;
    float eyeHeight = this->height / this->rows;
    this->eyeRadius = MIN(eyeWidth, eyeHeight);
    
    surfaceImg.load("surface1.png");
    whiteImg.load("white1.png");
    pupilImg.load("pupil1.png");
    shadeImg.load("shade1.png");
    
    setupEyes();
}

//--------------------------------------------------------------
void EyeGrid::update(const ofPoint& lookAt){
    for (auto & eye : eyes) {
        eye->lookAt(lookAt, MAX(this->width, this->height) * this->eyeRadius);
        eye->update();
    }
}

//--------------------------------------------------------------
void EyeGrid::draw(const bool& debugMode){
    
    eyesFbo.begin();
    ofClear(0, 0, 0, 0);
    ofSetColor(ofColor::white);
    for (auto & eye : eyes) {
        eye->draw(debugMode);
    }
    eyesFbo.end();
    
    eyesFbo.draw(0, 0, this->width, this->height);
}

//--------------------------------------------------------------
void EyeGrid::setupEyes(){
    eyesFbo.allocate(this->width, this->height);
    eyesFbo.begin();
    ofClear(0,0,0,0);
    ofSetColor(ofColor::white);
    eyesFbo.end();
    
    eyes.clear();
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
            
            ofLog(OF_LOG_VERBOSE, "Eye column:" + ofToString(j) + ", row:" + ofToString(i));
        }
    }
}

//--------------------------------------------------------------
void EyeGrid::setupEyes(int cols, int rows){
    this->cols = cols;
    this->rows = rows;
    this->setupEyes();
}

//--------------------------------------------------------------
void EyeGrid::rest() {
    for (auto & eye : eyes) {
        eye->rest();
    }
}

