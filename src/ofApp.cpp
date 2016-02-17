#include "ofApp.h"
#include "ofxAssimpUtils.h"

using namespace cv;
using namespace ofxCv;
using namespace eye;


//--------------------------------------------------------------
void ofApp::setup(){

    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetVerticalSync(true);
    ofBackground(0);
    ofSetFrameRate(60);
    ofSetWindowTitle("Bakarlar");
    
    float camW = 640;
    float camH = 480;
    
    this->eyeImageSet = shared_ptr<ImageSet> (new ImageSet);
    eyeImageSet->surface.load("surface1.png");
    eyeImageSet->white.load("white1.png");
    eyeImageSet->pupil.load("pupil1.png");
    eyeImageSet->shade.load("shade1.png");
    
    grabber.setup(camW, camH);
    setupParams();
    
    trackerFbo.allocate(camW, camH);
    trackerFbo.begin();
    ofClear(0, 0, 0, 0);
    trackerFbo.end();
    
    lookAtPoint.setDuration(.4);
    lookAtPoint.setPosition(ofPoint::zero());
    lookAtPoint.setRepeatType(PLAY_ONCE);
    lookAtPoint.setCurve(EASE_OUT);
    lookAtPoint.setRepeatTimes(0);

    bTracking = true;
}

//--------------------------------------------------------------
void ofApp::setupParams(){
    ofParameterGroup params;
    gridParams.setName("Grid");
    gridParams.add(cols.set("Columns", 7, 1, 16));
    gridParams.add(rows.set("Rows", 6, 1, 16));
    
    trackerParams.setName("Tracking");
    trackerParams.add(bTracking.set("Enabled", false));
    trackerParams.add(minAreaRadius.set("Min Area Radius", 1, 1, 20));
    trackerParams.add(maxAreaRadius.set("Max Area Radius", 100, 20, 250));
    trackerParams.add(threshold.set("Threshold", 15, 0, 255));
    trackerParams.add(persistence.set("Persistance", 15, 1, 50));
    trackerParams.add(maxDistance.set("Max Distance", 50, 1, 200));
    
    params.add(gridParams);
    params.add(trackerParams);
    params.add(bDebugMode.set("Debug", true));
    
    gui.setName("Settings");
    gui.setup(params);
    
    cols.addListener(this, &ofApp::setCols);
    rows.addListener(this, &ofApp::setRows);
    
    gui.loadFromFile("settings.xml");
    
    bDrawGui = true;
}

//--------------------------------------------------------------
void ofApp::update(){
    
    if (bTracking) {
        grabber.update();
        
        contourFinder.setMinAreaRadius(minAreaRadius);
        contourFinder.setMaxAreaRadius(maxAreaRadius);
        contourFinder.setThreshold(threshold);
        tracker.setPersistence(persistence);
        tracker.setMaximumDistance(maxDistance);
        
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
        for (auto & f : followers) {
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
    trackerFbo.draw(0, 0, trackerFbo.getWidth()/2, trackerFbo.getHeight()/2);
    
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