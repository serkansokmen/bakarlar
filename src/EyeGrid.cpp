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

//--------------------------------------------------------------
void Grid::lookAt(const ofPoint &lookAt){
    for (auto & eye : eyes) {
        eye->lookAt(lookAt);
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
            
            shared_ptr<Eye> eye(new Eye);
            // Set pixel data before eye setup
            eye->surfaceImg.setFromPixels(this->imageSet->surface.getPixels());
            eye->whiteImg.setFromPixels(this->imageSet->white.getPixels());
            eye->pupilImg.setFromPixels(this->imageSet->pupil.getPixels());
            eye->shadeImg.setFromPixels(this->imageSet->shade.getPixels());
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
        eye->rest();
    }
}

