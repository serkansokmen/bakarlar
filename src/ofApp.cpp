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
    ofSetEscapeQuitsApp(false);
    ofSetWindowShape(ofGetWidth() * 2, ofGetHeight() * 2);
    ofSetWindowPosition((ofGetScreenWidth() - ofGetWindowWidth())/2, (ofGetScreenHeight() - ofGetWindowHeight())/2);
    
    setupWebSocket();
    client.addListener(this);
    
    // setup osc
    receiver.setup(OSC_PORT);
    
    setupGui();
    
    gridRect.set(0, 0, ofGetWidth(), ofGetHeight());
    eyeGrid.setup(gridRect, eyeGrid.cols, eyeGrid.rows);
    
    loadSettings();
}

//--------------------------------------------------------------
void ofApp::setupWebSocket() {
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
}

//--------------------------------------------------------------
void ofApp::setupGui() {
    
    gui = new ofxDatGui(ofxDatGuiAnchor::TOP_RIGHT);
    
    gui->addHeader("Settings");
    gui->addButton(GUI_LABEL_CONNECT_TO_WS);
    
    ofxDatGuiFolder* gridFolder = new ofxDatGuiFolder("Eyes");
    gridFolder->addSlider(eyeGrid.cols);
    gridFolder->addSlider(eyeGrid.rows);
    
    gui->addFolder(gridFolder);
    gui->addFRM();
    
    gui->addToggle(GUI_LABEL_TOGGLE_DEBUG, eyeGrid.bDebugMode);
    
    gui->addFooter();
    gui->getFooter()->setLabelWhenExpanded("close");
    gui->getFooter()->setLabelWhenCollapsed("expand");
    
    gui->onButtonEvent([&](ofxDatGuiButtonEvent e) {
//        cout << e.target->getLabel() << endl;
        if (e.target->getLabel() == GUI_LABEL_CONNECT_TO_WS && !client.isConnected()) {
                setupWebSocket();
        }
        if (e.target->getLabel() == GUI_LABEL_TOGGLE_DEBUG) {
            eyeGrid.bDebugMode = !eyeGrid.bDebugMode;
        }
    });
    
    bDrawGui = true;
}

//--------------------------------------------------------------
void ofApp::update(){
    
    float dt = 1.0f / 60.0f;
    
    eyeGrid.update(poseVecs);
    
    gui->setAutoDraw(bDrawGui);
    
    while(receiver.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(m);
        if (m.getAddress() == "/debug") {
            bDrawGui = !bDrawGui;
        }
    }
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
    
//    if ((eyeGrid.cols != numPoses || eyeGrid.rows != numPoses) && numPoses > 0) {
//        eyeGrid.setup(gridRect, numPoses * 2, numPoses * 2);
//    }
    
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
            }
        }
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
        poseVecs.clear();
        
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
    if (eyeGrid.bDebugMode) {
        poseVecs.push_back(ofVec2f(x, y));
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
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
    saveSettings();
}

//--------------------------------------------------------------
void ofApp::loadSettings() {
    ofxXmlSettings settings;
    settings.loadFile(SETTINGS_FILE_NAME);
    
    eyeGrid.cols.set(settings.getValue("settings:cols", 4));
    eyeGrid.rows.set(settings.getValue("settings:rows", 4));
    eyeGrid.bDebugMode.set(settings.getValue("settings:debug", true));
}

//--------------------------------------------------------------
void ofApp::saveSettings() {
    ofxXmlSettings settings;
    settings.setValue("settings:cols", eyeGrid.cols);
    settings.setValue("settings:rows", eyeGrid.rows);
    settings.setValue("settings:debug", eyeGrid.bDebugMode);
    settings.saveFile(SETTINGS_FILE_NAME);
}
