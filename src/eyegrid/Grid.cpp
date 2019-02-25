#include "Grid.h"


using namespace eyegrid;

bool removeLookAt(const shared_ptr<ofxAnimatableOfPoint>& lookAt){
    return !lookAt->isAnimating();
}

//--------------------------------------------------------------
Grid::Grid(){
    params.setName("Grid");
    params.add(cols.set("Columns", 7, 1, 50));
    params.add(rows.set("Rows", 6, 1, 50));
    params.add(bDebugMode.set("Debug", true));
    
    imageSet = shared_ptr<EyeImageSet> (new EyeImageSet);
    imageSet->surface = shared_ptr<ofImage> (new ofImage);
    imageSet->white = shared_ptr<ofImage> (new ofImage);
    imageSet->pupil = shared_ptr<ofImage> (new ofImage);
    imageSet->shade = shared_ptr<ofImage> (new ofImage);
    imageSet->surface->load("surface1.png");
    imageSet->white->load("white1.png");
    imageSet->pupil->load("pupil1.png");
    imageSet->shade->load("shade1.png");
}

//--------------------------------------------------------------
Grid::~Grid(){
    cols.removeListener(this, &Grid::setCols);
    rows.removeListener(this, &Grid::setRows);
    
    eyes.clear();
}

//--------------------------------------------------------------
void Grid::setup(const ofRectangle& rect, int c, int r){
    
    float edgeLength = MIN(rect.getWidth(), rect.getHeight());
    
    this->rect.set(rect);
    this->cols = MAX((int)c, 1);
    this->rows = MAX((int)r, 1);
    
    float eyeWidth = rect.getWidth() / this->cols;
    float eyeHeight = rect.getHeight() / this->rows;
    this->eyeRadius = MIN(eyeWidth, eyeHeight);
    
    setupEyes();
    
    cols.addListener(this, &Grid::setCols);
    rows.addListener(this, &Grid::setRows);
}

//--------------------------------------------------------------
void Grid::setupEyes(){
    
    eyesFbo.allocate(rect.getWidth(), rect.getHeight());
    eyesFbo.begin();
    ofClear(0,0,0,0);
    ofSetColor(ofColor::white);
    eyesFbo.end();
    
    eyes.clear();
    
    for (int i = 0; i<(int)cols; i ++){
        
        for (int j = 0; j<(int)rows; j++){
            
            float x = ofLerp(rect.getX(), rect.getWidth(), (float)i / (float)cols);
            float y = ofLerp(rect.getY(), rect.getHeight(), (float)j / (float)rows);
            int index = j + i * cols;
            
            shared_ptr<Eye> eye(new Eye());
            eye->setup(ofVec2f(x, y), eyeRadius, eyeRadius);
            eye->setImageLayer(imageSet->surface, "surface");
            eye->setImageLayer(imageSet->white, "white");
            eye->setImageLayer(imageSet->pupil, "pupil");
            eye->setImageLayer(imageSet->shade, "shade");
            eyes.push_back(eye);
        }
    }
}

//--------------------------------------------------------------
void Grid::update(const vector<ofVec2f>& poseVecs) {
    
    int numPoses = poseVecs.size();
    
    for (int eyeIndex = 0; eyeIndex < cols * rows; ++eyeIndex) {
        auto eye = eyes[eyeIndex];
        
        if (numPoses > 0 && !eye->isAnimating()) {
            int poseIndex = (int)ofRandom(0, numPoses - 1);
            ofVec2f targetPos = poseVecs[poseIndex];
            float perc = ofNormalize(eyeRadius * EYE_PUPIL_POS_MULT, 0,
                                     rect.getHeight());
            ofVec2f eyeCenter = eye->eyeRect.getCenter();
            ofVec2f normalizedPos = eyeCenter.getInterpolated(targetPos, perc);
            eye->lookAt(normalizedPos);
//            ofLog(OF_LOG_NOTICE, "Pose Index: " + ofToString(poseIndex));
        } else {
            eye->rest();
        }
        
        eye->update();
    }
}

//--------------------------------------------------------------
void Grid::draw(){
    
    eyesFbo.begin();
    ofClear(0, 0, 0, 0);
    ofSetColor(ofColor::white);
    for (auto & eye : eyes) {
        eye->draw(bDebugMode);
    }
    eyesFbo.end();
    
    ofSetColor(ofColor::white);
    eyesFbo.draw(rect);
}

//--------------------------------------------------------------
//void Grid::lookAt(const ofPoint &lookAt){
//    
//    for (auto & eye : eyes) {
//        
//        float perc = ofNormalize(eyeRadius * EYE_PUPIL_POS_MULT, 0, rect.getHeight());
//        ofVec2f v = eye->restPos.getInterpolated(lookAt, perc);
//        eye->pupilPos.setDuration(ofRandom(0.8) + 0.2);
//        int rand = (int)ofRandom(0, 300);
//        switch (rand) {
//            case 0:
//                eye->pupilPos.setCurve(EASE_OUT);
//                break;
//            case 1:
//                eye->pupilPos.setCurve(LATE_EASE_IN_EASE_OUT);
//                break;
//            case 2:
//                eye->pupilPos.setCurve(VERY_LATE_LINEAR);
//                break;
//            default:
//                break;
//        }
//        if (!eye->pupilPos.isAnimating())
//            eye->pupilPos.animateTo(v);
//    }
//}


