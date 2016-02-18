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
    
    this->eyeImageSet = shared_ptr<ImageSet> (new ImageSet);
    eyeImageSet->surface.load("surface1.png");
    eyeImageSet->white.load("white1.png");
    eyeImageSet->pupil.load("pupil1.png");
    eyeImageSet->shade.load("shade1.png");
    
    fitGridRect();
    grabber.setup(GRABBER_WIDTH, GRABBER_HEIGHT);
    setupParams();
    
    trackerFbo.allocate(GRABBER_WIDTH, GRABBER_HEIGHT);
    trackerFbo.begin();
    ofClear(0, 0, 0, 0);
    trackerFbo.end();
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
        if (!grabber.isInitialized()) {
            grabber.setup(GRABBER_WIDTH, GRABBER_HEIGHT);
        }
        grabber.update();
        
        contourFinder.setMinAreaRadius(minAreaRadius);
        contourFinder.setMaxAreaRadius(maxAreaRadius);
        contourFinder.setThreshold(threshold);
        tracker.setPersistence(persistence);
        tracker.setMaximumDistance(maxDistance);
        
        if (grabber.isFrameNew()) {
            contourFinder.findContours(grabber);
            tracker.track(contourFinder.getBoundingRects());
            int contourCount = contourFinder.getBoundingRects().size();
            if (contourCount > 0) {
                ofPoint average(ofPoint::zero());
                for (auto & rect : contourFinder.getBoundingRects()) {
                    average.x += ofMap(rect.x, 0, GRABBER_WIDTH, gridRect.getX(), gridRect.getWidth());
                    average.y += ofMap(rect.y, 0, GRABBER_HEIGHT, gridRect.getY(), gridRect.getHeight());
                }
                average /= contourCount;
                eyeGrid.lookAt(average);
            } else {
                eyeGrid.rest();
            }
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
        
        grabber.close();
        
        trackerFbo.begin();
        ofClear(0, 0, 0, 0);
        trackerFbo.end();
    }
    
    eyeGrid.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetColor(ofColor::white);
    eyeGrid.draw(bDebugMode);
    
    trackerFbo.draw(0, 0, GRABBER_WIDTH/2, GRABBER_HEIGHT/2);
    
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
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    if (!bTracking) {
//        ofVec2f pos(ofMap(x, 0, ofGetWidth(), gridRect.getX(), gridRect.getWidth()),
//                    ofMap(y, 0, ofGetHeight(), gridRect.getY(), gridRect.getHeight()));
        eyeGrid.lookAt(ofVec2f(x, y));
    }
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    fitGridRect();
}

//--------------------------------------------------------------
void ofApp::exit(){
    cols.removeListener(this, &ofApp::setCols);
    rows.removeListener(this, &ofApp::setRows);
    gui.saveToFile("settings.xml");
}