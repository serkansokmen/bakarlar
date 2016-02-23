#include "ofApp.h"
#include "ofxAssimpUtils.h"


using namespace eyegrid;


//--------------------------------------------------------------
void ofApp::setup(){

    ofSetLogLevel(OF_LOG_WARNING);
    ofSetVerticalSync(true);
    ofBackground(0);
    ofSetFrameRate(60);
    ofSetWindowTitle("Bakarlar");
    ofEnableSmoothing();
    ofEnableAntiAliasing();
    
    setupParams();
    
    gridRect.set(0, 0, ofGetWidth(), ofGetHeight());
    eyeGrid.setup(gridRect, 4, 4);
    tracker.setup();
}

//--------------------------------------------------------------
void ofApp::setupParams(){
    ofParameterGroup params;
    params.add(tracker.params);
    params.add(eyeGrid.params);
    gui.setName("Settings");
    gui.setup(params);
    gui.loadFromFile("settings.xml");
    bDrawGui = true;
}

//--------------------------------------------------------------
void ofApp::update(){
    tracker.update();
    eyeGrid.update();
    
    int contourCount = tracker.getBoundingRects().size();
    if (contourCount > 0) {
        ofPoint average(ofPoint::zero());
        for (auto & rect : tracker.getBoundingRects()) {
            average.x += ofMap(rect.x, 0, GRABBER_WIDTH, gridRect.getX(), gridRect.getWidth());
            average.y += ofMap(rect.y, 0, GRABBER_HEIGHT, gridRect.getY(), gridRect.getHeight());
        }
        average /= contourCount;
        eyeGrid.lookAt(average);
    } else {
        if (tracker.isTracking()) {
            eyeGrid.rest();
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    eyeGrid.draw();
    tracker.draw(20, 20, 0.5f);
    
    if (bDrawGui) {
        gui.draw();
    }
}



//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 's') {
        bDrawGui = !bDrawGui;
    }
    if (key == 'f') {
        ofToggleFullscreen();
    }
    if (key == '=') {
        eyeGrid.cols ++;
        eyeGrid.rows ++;
    }
    if (key == '-') {
        eyeGrid.cols --;
        eyeGrid.rows --;
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    if (!tracker.isTracking()) {
        eyeGrid.lookAt(ofVec2f(x, y));
    }
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    float x = 0;
    float y = 0;
    gridRect.set(x, y, w, h);
    eyeGrid.fitEyesToRect(gridRect);
}

//--------------------------------------------------------------
void ofApp::exit(){
    gui.saveToFile("settings.xml");
}