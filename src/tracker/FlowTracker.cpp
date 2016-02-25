#include "FlowTracker.h"


using namespace tracker;


//--------------------------------------------------------------
FlowTracker::FlowTracker(){
    
    BaseTracker::BaseTracker();
    
    params.setName("Flow Tracker");
    params.add(lkMaxLevel.set("lkMaxLevel", 3, 0, 8));
    params.add(lkMaxFeatures.set("lkMaxFeatures", 200, 1, 1000));
    params.add(lkQualityLevel.set("lkQualityLevel", 0.01, 0.001, .02));
    params.add(lkMinDistance.set("lkMinDistance", 4, 1, 16));
    params.add(lkWinSize.set("lkWinSize", 8, 4, 64));
    params.add(usefb.set("Use Farneback", true));
    params.add(fbPyrScale.set("fbPyrScale", .5, 0, .99));
    params.add(fbLevels.set("fbLevels", 4, 1, 8));
    params.add(fbIterations.set("fbIterations", 2, 1, 8));
    params.add(fbPolyN.set("fbPolyN", 7, 5, 10));
    params.add(fbPolySigma.set("fbPolySigma", 1.5, 1.1, 2));
    params.add(fbUseGaussian.set("fbUseGaussian", false));
    params.add(fbWinSize.set("winSize", 32, 4, 64));
    
    curFlow = &fb;
}

//--------------------------------------------------------------
FlowTracker::~FlowTracker(){
}

//--------------------------------------------------------------
void FlowTracker::track(const ofPixels& pixels){
    
    if (usefb) {
        curFlow = &fb;
        fb.setPyramidScale(fbPyrScale);
        fb.setNumLevels(fbLevels);
        fb.setWindowSize(fbWinSize);
        fb.setNumIterations(fbIterations);
        fb.setPolyN(fbPolyN);
        fb.setPolySigma(fbPolySigma);
        fb.setUseGaussian(fbUseGaussian);
    } else {
        curFlow = &lk;
        lk.setMaxFeatures(lkMaxFeatures);
        lk.setQualityLevel(lkQualityLevel);
        lk.setMinDistance(lkMinDistance);
        lk.setWindowSize(lkWinSize);
        lk.setMaxLevel(lkMaxLevel);
    }
    
    ofImage img;
    img.setFromPixels(pixels);
    // you can use Flow polymorphically
    curFlow->calcOpticalFlow(img);
}

//--------------------------------------------------------------
void FlowTracker::draw(){
    BaseTracker::begin();
    curFlow->draw(0, 0, GRABBER_WIDTH, GRABBER_HEIGHT);
    BaseTracker::end();
    BaseTracker::draw();
}
