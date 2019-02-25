#include "ofApp.h"


using namespace eyegrid;

bool in_array(const std::string &value, const std::vector<string> &array){
    return std::find(array.begin(), array.end(), value) != array.end();
}


//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetLogLevel(OF_LOG_NOTICE);
    ofSetVerticalSync(true);
    ofBackground(0);
    ofSetFrameRate(60);
    ofSetWindowTitle("Bakarlar");
    ofEnableSmoothing();
    ofEnableAntiAliasing();
    
    // 1 - get default options
    ofxLibwebsockets::ClientOptions options = ofxLibwebsockets::defaultClientOptions();
    
    // 2 - set basic params
    options.host = CLIENT_HOST;
    options.port = CLIENT_PORT;
    
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
    
    // setup osc
    receiver.setup(OSC_PORT);
    
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
}

//--------------------------------------------------------------
void ofApp::update(){
    
    float dt = 1.0f / 60.0f;
    
    eyeGrid.update(poseVecs);
    
    while(receiver.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(m);
        if (m.getAddress() == "/debug") {
            bDrawGui = !bDrawGui;
        }
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
    
    if (eyeGrid.bDebugMode || bDrawGui) {
        for (auto &vec: poseVecs) {
            ofPushStyle();
            ofNoFill();
            ofSetLineWidth(2.0);
            ofSetColor(ofColor::greenYellow);
            ofDrawCircle(vec, 20);
            ofPopStyle();
        }
    }
    
    if (bDrawGui) {
        gui.draw();
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
//    ofLog(OF_LOG_NOTICE, args.message);
    
    int width = args.json["image"]["width"];
    int height = args.json["image"]["height"];
    
    int numPoses = args.json["poses"].size();
    
//    int numExistingPoses = poseVecs.size();
    poseVecs.clear();
    for (int poseIndex = 0; poseIndex < numPoses; ++poseIndex) {
        auto pose = args.json["poses"][poseIndex];
        auto keypoints = pose["keypoints"];
        float score = pose["score"];
        
        if (score < 0.2) { return; }
        
        for (auto &keypoint : keypoints) {
            auto partName = keypoint["part"];
            float partScore = keypoint["score"];
            
            if (in_array(partName, ALOWED_TRACK_PARTS) && partScore > 0.5) {
//            if (partName == "nose" && partScore > 0.5) {
                
                if (poseVecs.size() <= poseIndex) {
                    float x = keypoints[poseIndex]["position"]["x"];
                    float y = keypoints[poseIndex]["position"]["y"];
                    x = ofNormalize(x, 0, width);
                    y = ofNormalize(y, 0, height);
                    x *= ofGetWindowWidth();
                    y *= ofGetWindowHeight();
                    auto vec = ofVec2f(x, y);
                    poseVecs.push_back(vec);
                    break;
                }
                //                        poseVecs[poseIndex].set(vec);
                //                    break;
                //                average += vec;
            }
        }
        //        average = average / numPoses;
        //        average.x *= ofGetWindowWidth();
        //        average.y *= ofGetWindowHeight();
        
        //        poseVecs[poseIndex].set(average);
    }
}

//--------------------------------------------------------------
void ofApp::onBroadcast( ofxLibwebsockets::Event& args ){
    cout<<"got broadcast "<<args.message<<endl;
}



//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 's' || key == ofKey::OF_KEY_BACKSPACE) {
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
    if (bDrawGui) {
        //        for (auto &pos : poseVecs) {
        //            pos.set(x, y);
        //        }
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
    
    //    bUseGrabber.removeListener(this, &ofApp::toggleGrabber);
    //    bUsePlayer.removeListener(this, &ofApp::togglePlayer);
    //    bUseKinect.removeListener(this, &ofApp::toggleKinect);
    //
    //    player.close();
    //    kinect.close();
    gui.saveToFile("settings.xml");
}

