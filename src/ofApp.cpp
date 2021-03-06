#include "ofApp.h"


using namespace eyegrid;

bool removeLookAt(const shared_ptr<ofxAnimatableOfPoint>& lookAt){
    return !lookAt->isAnimating();
}

shared_ptr<ofxAnimatableOfPoint> getLookAt(const ofVec2f& vec) {
    auto lookAt = shared_ptr<ofxAnimatableOfPoint>(new ofxAnimatableOfPoint);
    lookAt->setDuration(ofRandom(0.8) + 0.2 + 0.8);
//    lookAt->setPosition(lastPositions[i]);
    lookAt->setRepeatType(PLAY_ONCE);
    lookAt->setRepeatTimes(0);
    int rand = (int)ofRandom(0, 3);
    switch (rand) {
        case 0:
            lookAt->setCurve(EASE_OUT);
            break;
        case 1:
            lookAt->setCurve(LATE_EASE_IN_EASE_OUT);
            break;
        case 2:
            lookAt->setCurve(VERY_LATE_LINEAR);
            break;
        default:
            break;
    }
    lookAt->animateTo(vec);
    return lookAt;
}


//--------------------------------------------------------------
void ofApp::setup(){

    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetVerticalSync(true);
    ofBackground(0);
    ofSetFrameRate(60);
    ofSetWindowTitle("Bakarlar");
    ofEnableSmoothing();
    ofEnableAntiAliasing();
    
    // 1 - get default options
    ofxLibwebsockets::ClientOptions options = ofxLibwebsockets::defaultClientOptions();
    
    // 2 - set basic params
    options.host = CLIENT;
    options.port = PORT;
    
    // advanced: set keep-alive timeouts for events like
    // loss of internet
    
    // 3 - set keep alive params
    // BIG GOTCHA: on BSD systems, e.g. Mac OS X, these time params are system-wide
    // ...so ka_time just says "check if alive when you want" instead of "check if
    // alive after X seconds"
    options.ka_time     = 1;
    options.ka_probes   = 1;
    options.ka_interval = 1;
    
    // 4 - connect
    client.connect(options);
    client.addListener(this);
    
    // Setup params
    gui.setName("Settings");
    ofParameterGroup params;
    
    params.add(eyeGrid.params);
    gui.setup(params);
    
//    datGui = new ofxDatGui(ofxDatGuiAnchor::TOP_LEFT);
//    datGui->addSlider(eyeGrid.params);
//    datGui->addToggle("Debug", bDrawGui);
    
    gui.loadFromFile("settings.xml");
    bDrawGui = true;
    
    gridRect.set(0, 0, ofGetWidth(), ofGetHeight());
    eyeGrid.setup(gridRect, eyeGrid.cols, eyeGrid.rows);
    for (int i=0; i<eyeGrid.cols * eyeGrid.rows; ++i) {
        lookAtPositions.push_back(getLookAt(ofGetWindowSize() / 2));
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    
    eyeGrid.update(lookAtPositions);
//    datGui->setAutoDraw(bDrawGui);
    
    float dt = 1.0f / 60.0f;
    
//    ofRemove(lookAtPositions, removeLookAt);
    for (int i=0; i<lookAtPositions.size(); ++i) {
        lookAtPositions[i]->update(dt);
    }
    
//    if (!lookAt->isAnimating()){
//        eyeGrid.lookAt(ofVec2f(lookAt->getCurrentPosition().x, lookAt->getCurrentPosition().y));
//    } else {
//        // TODO: add behaivoral lookAt
//        eyeGrid.rest();
//    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    eyeGrid.draw();
    
    if (bDrawGui) {
//        ofDrawCircle(lookAt->getCurrentPosition(), 20);
        gui.draw();
        
        for (auto &pos: lookAtPositions) {
            if (pos->isAnimating()) {
                ofDrawCircle(pos->getCurrentPosition(), 10);
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::onConnect( ofxLibwebsockets::Event& args ){
    cout<<"on connected"<<endl;
}

//--------------------------------------------------------------
void ofApp::onOpen( ofxLibwebsockets::Event& args ){
    cout<<"on open"<<endl;
}

//--------------------------------------------------------------
void ofApp::onClose( ofxLibwebsockets::Event& args ){
    cout<<"on close"<<endl;
}

//--------------------------------------------------------------
void ofApp::onIdle( ofxLibwebsockets::Event& args ){
    cout<<"on idle"<<endl;
}

//--------------------------------------------------------------
void ofApp::onMessage( ofxLibwebsockets::Event& args ){
    ofLog(OF_LOG_NOTICE, args.message);
    
    int width = args.json["image"]["width"];
    int height = args.json["image"]["height"];
    
    int numPoses = args.json["poses"].size();
//    lastPositions.resize(numPoses);
    
    for (int poseIndex = 0; poseIndex < numPoses; ++poseIndex) {
        auto pose = args.json["poses"][poseIndex];
        auto keypoints = pose["keypoints"];
        auto partName = keypoints[poseIndex]["part"];

        if (partName == "nose") {
            if (!lookAtPositions[poseIndex]->isAnimating()) {
                float x = keypoints[poseIndex]["position"]["x"];
                float y = keypoints[poseIndex]["position"]["y"];
                x = ofNormalize(x, 0, width) * ofGetWidth();
                y = ofNormalize(y, 0, height) * ofGetHeight();
                auto vec = ofVec2f(x, y);
//                auto lookAt = getLookAt(vec);
                lookAtPositions[poseIndex]->animateTo(vec);
            }
//            lastPositions[i].set(vec);
        }
        
        ofLog(OF_LOG_VERBOSE, partName);
    }
}

//--------------------------------------------------------------
void ofApp::onBroadcast( ofxLibwebsockets::Event& args ){
    cout<<"got broadcast "<<args.message<<endl;
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
//    ofRectangle rect(colorTracker.drawPos.get(), GRABBER_WIDTH, GRABBER_HEIGHT);
//    if (isTracking() && rect.inside(x, y)) {
//        if (bUseGrabber) {
//            *trackPixels = grabber.getPixels();
//        } else if (bUsePlayer) {
//            *trackPixels = player.getPixels();
//        } else if (bUseKinect) {
//            *trackPixels = kinect.getDepthPixels();
//        };
//        colorTracker.targetColor = trackPixels->getColor(x - colorTracker.drawPos->x, y - colorTracker.drawPos->y);
//    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
//    if (!isTracking()) {
//        lookAt->animateTo(ofVec2f(x, y));
//    }
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
    
//    bUseGrabber.removeListener(this, &ofApp::toggleGrabber);
//    bUsePlayer.removeListener(this, &ofApp::togglePlayer);
//    bUseKinect.removeListener(this, &ofApp::toggleKinect);
//
//    player.close();
//    kinect.close();
    gui.saveToFile("settings.xml");
}
