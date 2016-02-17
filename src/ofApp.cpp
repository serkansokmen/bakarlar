#include "ofApp.h"
#include "ofxAssimpUtils.h"

using namespace cv;
using namespace ofxCv;


//--------------------------------------------------------------
void ofApp::setup(){

    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetVerticalSync(true);
    ofBackground(0);
    ofSetFrameRate(60);
    ofSetWindowTitle("Bakarlar");
    
    float camW = 640;
    float camH = 480;
    
    trackerFbo.allocate(camW, camH);
    trackerFbo.begin();
    ofClear(0, 0, 0, 0);
    trackerFbo.end();
    
    grabber.setup(camW, camH);
    threshold = 15.f;
    contourFinder.setMinAreaRadius(1);
    contourFinder.setMaxAreaRadius(100);
    contourFinder.setThreshold(threshold);
    
    // wait for half a frame before forgetting something
    tracker.setPersistence(15);
    // an object can move up to 50 pixels per frame
    tracker.setMaximumDistance(50);
    
    lookAtPoint.setDuration(.4);
    lookAtPoint.setPosition(ofPoint::zero());
    lookAtPoint.setRepeatType(PLAY_ONCE);
    lookAtPoint.setCurve(EASE_OUT);
    lookAtPoint.setRepeatTimes(0);

    bTracking = true;
    
    gui.setup();
    gui.setName("Settings");
    gui.add(cols.set("Columns", 7, 1, 16));
    gui.add(rows.set("Rows", 6, 1, 16));
    gui.add(threshold.set("Threshold", 15, 0, 255));
    gui.add(bTracking.set("Track", false));
    gui.add(bDebugMode.set("Debug", true));
    
    cols.addListener(this, &ofApp::setCols);
    rows.addListener(this, &ofApp::setRows);
    
    gui.loadFromFile("settings.xml");
    
    bDrawGui = true;
}

//--------------------------------------------------------------
void ofApp::update(){
    
    if (bTracking) {
        grabber.update();
        if (grabber.isFrameNew()) {
            contourFinder.findContours(grabber);
            tracker.track(contourFinder.getBoundingRects());
        }
        
        trackerFbo.begin();
        ofClear(0, 0, 0, 0);
        ofSetColor(ofColor::white);
        grabber.draw(0, 0);
        contourFinder.draw();
        vector<Glow>& followers = tracker.getFollowers();
        for (auto f : followers) {
            f.draw();
        }
        trackerFbo.end();
    } else {
        trackerFbo.begin();
        ofClear(0, 0, 0, 0);
        trackerFbo.end();
    }
    
    float dt = 1.0f / 60.0f;
    lookAtPoint.update(dt);
    
    eyeGrid.update(lookAtPoint.getCurrentPosition());
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetColor(ofColor::white);
    eyeGrid.draw(bDebugMode);
    
    lookAtPoint.draw();
    
    trackerFbo.draw(0, 0, 320, 240);
    
    if (bDrawGui) {
        gui.draw();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 's') {
        bDrawGui = !bDrawGui;
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    lookAtPoint.animateTo(ofVec2f(x, y));
}

//--------------------------------------------------------------
void ofApp::exit(){
    cols.removeListener(this, &ofApp::setCols);
    rows.removeListener(this, &ofApp::setRows);
    
    gui.saveToFile("settings.xml");
}