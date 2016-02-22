#include "Grid.h"


using namespace eyegrid;


//--------------------------------------------------------------
Grid::Grid(){
    params.setName("Grid");
    params.add(cols.set("Columns", 7, 1, 16));
    params.add(rows.set("Rows", 6, 1, 16));
    params.add(bDebugMode.set("Debug", true));
}

//--------------------------------------------------------------
Grid::~Grid(){
    cols.removeListener(this, &Grid::setCols);
    rows.removeListener(this, &Grid::setRows);
    
    eyes.clear();
}

//--------------------------------------------------------------
void Grid::setup(const ofRectangle& rect, int c, int r, shared_ptr<EyeImageSet> set){
    
    float edgeLength = MIN(rect.getWidth(), rect.getHeight());
    
    this->rect.set(rect);
    this->cols = MAX((int)c, 1);
    this->rows = MAX((int)r, 1);
    
    float eyeWidth = rect.getWidth() / this->cols;
    float eyeHeight = rect.getHeight() / this->rows;
    this->eyeRadius = MIN(eyeWidth, eyeHeight);
    
    this->imageSet = set;
    
    this->init();
    
    cols.addListener(this, &Grid::setCols);
    rows.addListener(this, &Grid::setRows);
}

//--------------------------------------------------------------
void Grid::update(){
    for (auto & eye : eyes) {
        eye->update();
    }
}

//--------------------------------------------------------------
void Grid::draw(){
    
    eyesFbo.begin();
    ofClear(0, 0, 0, 0);
    ofSetColor(ofColor::white);
    for (auto & eye : eyes) {
        eye->draw(this->bDebugMode);
    }
    eyesFbo.end();
    
    ofSetColor(ofColor::white);
    eyesFbo.draw(this->rect);
}

#pragma mark

//--------------------------------------------------------------
void Grid::init(){
    eyesFbo.allocate(this->rect.getWidth(), this->rect.getHeight());
    eyesFbo.begin();
    ofClear(0,0,0,0);
    ofSetColor(ofColor::white);
    eyesFbo.end();
    
    eyes.clear();
    for (int i = 0; i<(int)this->cols; i ++){
        
        float colPerc = (float)i / (float)this->cols;
        
        for (int j = 0; j<(int)this->rows; j++){
            
            float rowPerc = (float)j / (float)this->rows;
            float x = ofLerp(this->rect.getX(), this->rect.getWidth(), colPerc);
            float y = ofLerp(this->rect.getY(), this->rect.getHeight(), rowPerc);
            ofVec2f pos(x, y);
            
            shared_ptr<Eye> eye(new Eye(this->imageSet));
            eye->setup(pos, this->eyeRadius, this->eyeRadius);
            eyes.push_back(eye);
        }
    }
}

//--------------------------------------------------------------
void Grid::lookAt(const ofPoint &lookAt){
    
    for (auto & eye : eyes) {
        
        float perc = ofNormalize(eyeRadius * EYE_PUPIL_MULT, 0, this->rect.getHeight());
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
void Grid::rest() {
    for (auto & eye : eyes) {
        eye->pupilPos.animateTo(eye->restPos);
    }
}

