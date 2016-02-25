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
    player.load("video.mov");
    trackPixels = unique_ptr<ofPixels>(new ofPixels);
    
    gridRect.set(0, 0, ofGetWidth(), ofGetHeight());
    eyeGrid.setup(gridRect, 4, 4);
    
    lookAt = unique_ptr<ofxAnimatableOfPoint>(new ofxAnimatableOfPoint);
    lookAt->setPosition(ofPoint::zero());
    lookAt->setRepeatType(PLAY_ONCE);
    lookAt->setRepeatTimes(0);
    lookAt->setCurve(EASE_IN_EASE_OUT);
    
    // Setup params
    gui.setName("Settings");
    ofParameterGroup params;
    params.add(bUseGrabber.set("Use Video Grabber", false));
    params.add(bUsePlayer.set("Use Video Player", false));
    params.add(bUseKinect.set("Use Kinect", false));
    params.add(colorTracker.params);
    params.add(flowTracker.params);
    params.add(eyeGrid.params);
    gui.setup(params);
    
    bUseGrabber.addListener(this, &ofApp::toggleGrabber);
    bUsePlayer.addListener(this, &ofApp::togglePlayer);
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
    if (bUsePlayer){
        player.update();
        if (player.isFrameNew()) {
            colorTracker.track(player.getPixels());
            flowTracker.track(player.getPixels());
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
    if (isTracking()) {
        if (colorTracker.enabled){
            int contourCount = colorTracker.getBoundingRects().size();
            if (contourCount > 0 && isTracking()){
                unique_ptr<ofPoint> p = unique_ptr<ofPoint>(new ofPoint(colorTracker.getAverage()));
                
                p->x = ofNormalize(colorTracker.getAverage().x, colorTracker.drawPos->x, GRABBER_WIDTH);
                p->y = ofNormalize(colorTracker.getAverage().y, colorTracker.drawPos->y, GRABBER_HEIGHT);
                ofLogWarning(ofToString(*p));
                if (!lookAt->isAnimating())
                    lookAt->animateTo(*p);
            }
        }
    }
    
    if (lookAt->isAnimating()){
        eyeGrid.lookAt(ofVec2f(lookAt->getCurrentPosition().x, lookAt->getCurrentPosition().y));
    } else {
        eyeGrid.rest();
    }
}

//--------------------------------------------------------------
void ofApp::toggleGrabber(bool& yes) {
    if (yes) {
        grabber.setup(GRABBER_WIDTH, GRABBER_HEIGHT);
    } else {
        if (grabber.isInitialized()) {
            grabber.close();
        }
    }
}

//--------------------------------------------------------------
void ofApp::togglePlayer(bool& yes) {
    if (yes) {
        player.play();
    } else {
        player.stop();
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
    
    ofSetColor(ofColor::red);
    ofDrawCircle(lookAt->getCurrentPosition(), 20);
    ofSetColor(ofColor::white);
    
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
    if (key == ' ') {
        player.setPaused(player.isPlaying());
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    ofRectangle rect(colorTracker.drawPos.get(), GRABBER_WIDTH, GRABBER_HEIGHT);
    if (isTracking() && rect.inside(x, y)) {
        if (bUseGrabber) {
            *trackPixels = grabber.getPixels();
        } else if (bUsePlayer) {
            *trackPixels = player.getPixels();
        } else if (bUseKinect) {
            *trackPixels = kinect.getDepthPixels();
        };
        colorTracker.targetColor = trackPixels->getColor(x - colorTracker.drawPos->x, y - colorTracker.drawPos->y);
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    if (!isTracking()) {
        lookAt->animateTo(ofVec2f(x, y));
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
    bUsePlayer.removeListener(this, &ofApp::togglePlayer);
    bUseKinect.removeListener(this, &ofApp::toggleKinect);
    
    player.close();
    kinect.close();
    gui.saveToFile("settings.xml");
}