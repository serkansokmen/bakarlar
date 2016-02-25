#include "ColorTracker.h"


using namespace tracker;


//--------------------------------------------------------------
ColorTracker::ColorTracker(){

    BaseTracker::BaseTracker();
    
    params.setName("Contour Tracker");
    params.add(minAreaRadius.set("Min Area Radius", 10.0, 1.0, min(GRABBER_WIDTH, GRABBER_HEIGHT)));
    params.add(maxAreaRadius.set("Max Area Radius", 200.0, 1.0, min(GRABBER_WIDTH, GRABBER_HEIGHT)));
    params.add(threshold.set("Threshold", 50, 0, 255));
    params.add(targetColor.set("Track Color", ofColor::white, ofColor::black, ofColor::white));
    params.add(blur.set("Blur", 4, 0, 20));
    
    minAreaRadius.addListener(this, &ColorTracker::setMinAreaRadius);
    maxAreaRadius.addListener(this, &ColorTracker::setMaxAreaRadius);
    threshold.addListener(this, &ColorTracker::setThreshold);
    targetColor.addListener(this, &ColorTracker::setTrackingColor);
    
    colorImage.allocate(GRABBER_WIDTH, GRABBER_HEIGHT, OF_IMAGE_COLOR);
    
    trackingColorMode = ofxCv::TRACK_COLOR_HSV;
}

//--------------------------------------------------------------
ColorTracker::~ColorTracker(){
    minAreaRadius.removeListener(this, &ColorTracker::setMinAreaRadius);
    maxAreaRadius.removeListener(this, &ColorTracker::setMaxAreaRadius);
    threshold.removeListener(this, &ColorTracker::setThreshold);
    targetColor.removeListener(this, &ColorTracker::setTrackingColor);
}

//--------------------------------------------------------------
void ColorTracker::track(const ofPixels& pixels){
    if (enabled) {
        
        colorImage.setFromPixels(pixels);
        ofxCv::GaussianBlur(colorImage, blur);
        colorImage.update();
        
        contourFinder.findContours(colorImage);
    }
}

//--------------------------------------------------------------
void ColorTracker::draw(){
    if (enabled){
        BaseTracker::begin();
        colorImage.draw(0, 0);
        contourFinder.draw();
        ofSetColor(255);
        BaseTracker::end();
        BaseTracker::draw();
    }
}

//--------------------------------------------------------------
void ColorTracker::setMinAreaRadius(float& val) {
    contourFinder.setMinAreaRadius(val);
}

//--------------------------------------------------------------
void ColorTracker::setMaxAreaRadius(float& val) {
    contourFinder.setMaxAreaRadius(val);
}

//--------------------------------------------------------------
void ColorTracker::setThreshold(float& val) {
    contourFinder.setThreshold(val);
}

//--------------------------------------------------------------
void ColorTracker::setTrackingColor(ofColor& color) {
    contourFinder.setTargetColor(color, trackingColorMode);
}
