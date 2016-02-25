#include "ColorTracker.h"


using namespace tracker;


//--------------------------------------------------------------
ColorTracker::ColorTracker(){

    BaseTracker::BaseTracker();
    
    params.setName("Contour Tracker");
    params.add(minAreaNorm.set("Min Area Norm", 0.7, 0.0, 1.0));
    params.add(threshold.set("Threshold", 50, 0, 255));
    params.add(targetColor.set("Track Color", ofColor::white, ofColor::black, ofColor::white));
    params.add(blur.set("Blur", 4, 0, 20));
    
    targetColor.addListener(this, &ColorTracker::setTrackingColor);
    
    grayImage.allocate(GRABBER_WIDTH, GRABBER_HEIGHT, OF_IMAGE_GRAYSCALE);
    grayThreshNear.allocate(GRABBER_WIDTH, GRABBER_HEIGHT, OF_IMAGE_GRAYSCALE);
    grayThreshFar.allocate(GRABBER_WIDTH, GRABBER_HEIGHT, OF_IMAGE_GRAYSCALE);
    grayPreprocImage.allocate(GRABBER_WIDTH, GRABBER_HEIGHT, OF_IMAGE_GRAYSCALE);
    
    contourFinder.setFindHoles(true);
    //    contourFinder.setSimplify(true);
    trackingColorMode = ofxCv::TRACK_COLOR_RGB;
}

//--------------------------------------------------------------
ColorTracker::~ColorTracker(){
    targetColor.removeListener(this, &ColorTracker::setTrackingColor);
}

//--------------------------------------------------------------
void ColorTracker::track(const ofPixels& pixels){
    if (enabled) {
        colorImage.setFromPixels(pixels);
        ofxCv::convertColor(colorImage, src, CV_RGB2GRAY);
        src = ofxCv::toCv(colorImage);
        ofxCv::GaussianBlur(src, blur);
        ofxCv::toOf(src, grayPreprocImage);
        grayPreprocImage.update();
        
        contourFinder.setMinAreaNorm(minAreaNorm);
        contourFinder.setThreshold(threshold);
        contourFinder.findContours(grayPreprocImage);
    }
}

//--------------------------------------------------------------
void ColorTracker::draw(){
    if (enabled){
        BaseTracker::begin();
        grayPreprocImage.draw(0, 0);
        contourFinder.draw();
        BaseTracker::end();
        BaseTracker::draw();
    }
}
