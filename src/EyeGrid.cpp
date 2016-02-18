#include "EyeGrid.h"


using namespace eye;


//--------------------------------------------------------------
Grid::~Grid(){
    eyes.clear();
}

//--------------------------------------------------------------
void Grid::setup(const ofRectangle& rect, int c, int r, shared_ptr<ImageSet> set){
    
    float edgeLength = MIN(rect.getWidth(), rect.getHeight());
    
    this->width = rect.getWidth();
    this->height = rect.getHeight();
    this->cols = MAX((int)c, 1);
    this->rows = MAX((int)r, 1);
    
    float eyeWidth = this->width / this->cols;
    float eyeHeight = this->height / this->rows;
    this->eyeRadius = MIN(eyeWidth, eyeHeight);
    
    this->imageSet = set;
    
    this->init();
}

//--------------------------------------------------------------
void Grid::update(){
    for (auto & eye : eyes) {
        eye->update();
    }
}

void Grid::lookAt(const ofPoint &lookAt){
    for (auto & eye : eyes) {
        float perc = ofNormalize(eyeRadius * .25, 0, this->height) * 0.85;
        ofVec2f v = eye->restPos.getInterpolated(lookAt, perc);
        eye->pupilPos.setDuration(ofRandom(0.8) + 0.2);
        int rand = (int)ofRandom(0, 300);
        switch (rand) {
            case 0:
                eye->pupilPos.setCurve(EASE_OUT);
                break;
            case 1:
                eye->pupilPos.setCurve(LATE_EASE_IN_EASE_OUT);
                break;
            case 2:
                eye->pupilPos.setCurve(VERY_LATE_LINEAR);
                break;
            default:
                break;
        }
        eye->pupilPos.animateTo(v);
    }
}

//--------------------------------------------------------------
void Grid::init(){
    eyesFbo.allocate(this->width, this->height);
    eyesFbo.begin();
    ofClear(0,0,0,0);
    ofSetColor(ofColor::white);
    eyesFbo.end();
    
    eyes.clear();
    for (int i = 0; i<(int)this->cols; i ++){
        for (int j = 0; j<(int)this->rows; j++){
            
            shared_ptr<Eye> eye(new Eye(this->imageSet));
            eye->setup(ofVec2f(i*this->eyeRadius, j*this->eyeRadius),
                       this->eyeRadius,
                       this->eyeRadius);
            eyes.push_back(eye);
            
            ofLog(OF_LOG_VERBOSE, "Eye column:" + ofToString(j) + ", row:" + ofToString(i));
        }
    }
}

//--------------------------------------------------------------
void Grid::draw(const bool& debugMode){
    
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
void Grid::rest() {
    for (auto & eye : eyes) {
        eye->pupilPos.animateTo(eye->restPos);
    }
}

