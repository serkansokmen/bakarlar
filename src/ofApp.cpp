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
    
    kinect.init();
    kinect.setRegistration(true);
    
    setupParams();
    
    gridRect.set(0, 0, ofGetWidth(), ofGetHeight());
    eyeGrid.setup(gridRect, 4, 4);
    
    lookAt = unique_ptr<ofPoint>(new ofPoint);
}

//--------------------------------------------------------------
void ofApp::setupParams(){
    gui.setName("Settings");
    ofParameterGroup params;
    params.add(bUseGrabber.set("Use Video Grabber", false));
    params.add(bUseKinect.set("Use Kinect", false));
    params.add(colorTracker.params);
    params.add(flowTracker.params);
    params.add(eyeGrid.params);
    gui.setup(params);
    
    bUseGrabber.addListener(this, &ofApp::toggleGrabber);
    bUseKinect.addListener(this, &ofApp::toggleKinect);
    
    gui.loadFromFile("settings.xml");
    bDrawGui = true;
}

//--------------------------------------------------------------
void ofApp::update(){
    
    if (bUseGrabber){
        grabber.update();
        if (grabber.isFrameNew()) {
            colorTracker.track(grabber.getPixels());
            flowTracker.track(grabber.getPixels());
        }
    }
    if (bUseKinect){
        kinect.update();
        // there is a new frame and we are connected
        if (kinect.isFrameNew()) {
            colorTracker.track(kinect.getDepthPixels());
            flowTracker.track(kinect.getDepthPixels());
        }
    }
    
    eyeGrid.update();
    
//    int contourCount = colorTracker.getBoundingRects().size();
//    if (contourCount > 0 && isTracking()){
////        *lookAt = colorTracker.getAverage();
//        lookAt->x = ofMap(lookAt->x, 0, GRABBER_WIDTH, 0, ofGetWidth());
//        lookAt->y = ofMap(lookAt->y, 0, GRABBER_HEIGHT, 0, ofGetHeight());
//        eyeGrid.lookAt(*lookAt);
//    } else {
//        if (isTracking()){
//            eyeGrid.rest();
//        }
//    }
}

//--------------------------------------------------------------
void ofApp::toggleGrabber(bool& yes) {
    ofLog(OF_LOG_NOTICE, ofToString(yes));
    if (yes) {
        grabber.setup(GRABBER_WIDTH, GRABBER_HEIGHT);
    } else {
        if (grabber.isInitialized()) {
            grabber.close();
        }
    }
}

//--------------------------------------------------------------
void ofApp::toggleKinect(bool& yes) {
    if (yes) {
        kinect.init();
        kinect.setRegistration(true);
        kinect.open();
    } else {
        kinect.close();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    eyeGrid.draw();
    colorTracker.draw();
    flowTracker.draw();
    
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
void ofApp::mousePressed(int x, int y, int button){
    ofRectangle rect(colorTracker.drawPos.get(), GRABBER_WIDTH, GRABBER_HEIGHT);
    if (isTracking() && rect.inside(x, y)) {
        ofColor color(grabber.getPixels().getColor(x - colorTracker.drawPos->x, y - colorTracker.drawPos->y));
        colorTracker.targetColor = color;
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    if (!isTracking()) {
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
    
    bUseGrabber.removeListener(this, &ofApp::toggleGrabber);
    bUseKinect.removeListener(this, &ofApp::toggleKinect);
    grabber.close();
    kinect.close();
    gui.saveToFile("settings.xml");
}