#include "testApp.h"

int camWidth = 640;
int camHeight = 480;


//--------------------------------------------------------------
void testApp::setup(){

    ofSetBackgroundAuto(true);
	ofEnableAlphaBlending();
	ofBackground(0);
	ofSetFrameRate(50);
	ofSetVerticalSync(true);
    ofEnableSmoothing();
    ofSetLogLevel(OF_LOG_VERBOSE);
	
    bEyesInitialized = false;
    
    surfaceImg.loadImage("surface1.png");
	whiteImg.loadImage("white1.png");
	pupilImg.loadImage("pupil1.png");
	shadeImg.loadImage("shade1.png");
    
    eyesFbo.allocate(ofGetWidth(), ofGetHeight());

#ifdef USE_OSC
    oscReceiver.setup(8000);
#endif

#ifdef USE_BLOB_DETECTION
    threshold = 17.5f;
    fade = 27.0f;
    bDrawFlob = true;
    bDebugMode = true;
    
    blobs = NULL;
	vidGrabber.setVerbose(true);
	vidGrabber.listDevices();
	vidGrabber.initGrabber(camWidth, camHeight);
#endif
    
    // Setup GUI here to load previously saved settings
    setupGui();
    
#ifdef USE_BLOB_DETECTION
    flob.setup((int)vidGrabber.getWidth(), (int)vidGrabber.getHeight(), ofGetWidth(), ofGetHeight());
	//flob.setup(camWidth/2, camHeight/2, (float)ofGetWidth(), (float)ofGetHeight());
    
	flob.setOm(Flob::CONTINUOUS_DIFFERENCE)
        ->setColormode(Flob::LUMA601)
        ->setFade(fade)->setThresh(threshold)
        ->setThresholdmode(Flob::ABSDIF);
    flob.mirrorX = true;
    flob.mirrorY = false;
#endif
    
    // Prevent jumpy tweens
    Tweener.setMode(TWEENMODE_OVERRIDE);
    
    initEyes();
}

//--------------------------------------------------------------
void testApp::update(){
    
    Tweener.update();
    
    eyeCountHorizontal = (int)eyeCountHorizontal;
    eyeCountVertical = (int)eyeCountVertical;
    
    if (!bEyesInitialized){
        eyes.clear();
		initEyes();
	} else {
        updateEyes();
	}
}

//--------------------------------------------------------------
void testApp::draw(){

    if (bEyesInitialized) {
        eyesFbo.begin();
        ofBackground(0);
        for (int i=0; i<eyes.size(); i++) {
            eyes[i]->draw(&bDebugMode);
        }
        eyesFbo.end();
    }
    
    eyesFbo.draw(0, 0, ofGetWidth(), ofGetHeight());
    
#ifdef USE_BLOB_DETECTION
    if (bDrawFlob) {
        //        ofSetColor(255, 75);
        //        flob.draw();
        
        if (blobs != NULL){
            for (int i=0; i<blobs->size();i++){
                ABlob & ab  = *(blobs->at(i));
                ofPushStyle();
                ofNoFill();
                ofSetColor(0, 0, 255, 200);
                ofRect(ab.bx, ab.by, ab.dimx, ab.dimy);
                ofFill();
                ofSetColor(0, 255, 0, 150);
                ofRect(ab.cx, ab.cy, 5, 5);
                ofPopStyle();
            }
        }
    }
#endif
    
    if (bDebugMode) {
        ofPushStyle();
        ofNoFill();
        ofSetColor(0, 175, 0, 255);
        ofCircle(debugCirclePos, 40);
        ofFill();
        ofSetColor(255, 25);
        ofCircle(debugCirclePos, 40);
        ofPopStyle();
        gui->setVisible(true);
    } else {
        gui->setVisible(false);
    }
    
    if (gui->isVisible()) {
        gui->draw();
        
#ifdef USE_BLOB_DETECTION
        flob.videotexmotion.draw(10, ofGetHeight() - camHeight + 120, camWidth/4, camHeight/4);
#endif
    }
}

//--------------------------------------------------------------
void testApp::initEyes(){
    
    float eyeWidth = ofGetWidth() / MAX((int)eyeCountHorizontal, 1);
	float eyeHeight = ofGetHeight() / MAX((int)eyeCountVertical, 1);
    
    eyeSize = MIN(eyeWidth, eyeHeight);
    
    for (int i = 0; i<(int)eyeCountHorizontal; i ++){
		for (int j = 0; j<(int)eyeCountVertical; j++){

			Eye * eye = new Eye;
            // Set pixel data before eye setup
			eye->surfaceImg.setFromPixels(surfaceImg.getPixelsRef());
            eye->whiteImg.setFromPixels(whiteImg.getPixelsRef());
            eye->pupilImg.setFromPixels(pupilImg.getPixelsRef());
            eye->shadeImg.setFromPixels(shadeImg.getPixelsRef());
            
            float posX = i*eyeWidth;
            float posY = j*eyeHeight;
            
            eye->setup(ofVec2f(posX, posY), eyeSize, eyeSize);
			eyes.push_back(eye);
            
            ofLog(OF_LOG_VERBOSE, "Added eye j:" + ofToString(j) + ", i:" + ofToString(i));
		}
	}
    bEyesInitialized = true;
    
    ofLog(OF_LOG_NOTICE, "eye initialization finished.");
}

//--------------------------------------------------------------
void testApp::updateEyes(){
    
    float lookX = ofGetWidth() * .5;
    float lookY = ofGetHeight() * .5;
    
#ifdef USE_BLOB_DETECTION
    
    vidGrabber.update();
    ofVec2f lookAtVec;
    
    if (vidGrabber.isFrameNew()){
        blobs = flob.calc(flob.binarize(vidGrabber.getPixels(), (int)vidGrabber.getWidth(), (int)vidGrabber.getHeight()));
        
        // Calculate centroid
        if (blobs != NULL && blobs->size() > 0){
            
            ofVec2f *tempVec = new ofVec2f;
            
            for (int n=0; n<blobs->size(); n++) {
                
                ABlob &blob = *(blobs->at(n));
                
                tempVec->set(blob.cx, blob.cy);
                lookAtVec = lookAtVec + *tempVec;
            }
            lookAtVec = lookAtVec / blobs->size();
            
            delete tempVec;
            
        } else {
            lookAtVec.set(ofGetWidth()/2, ofGetHeight()/2);
        }
        lookAtCentroid.set(lookAtVec);
        
        for (int i=0; i<eyes.size(); i++){
            eyes[i]->lookAt(lookAtCentroid);
        }
    } else {
        for (int i=0; i<eyes.size(); i++){
            eyes[i]->rest();
        }
    }
#endif
    
#ifdef USE_OSC
    float posX = ofGetWidth()*.5;
    float posY = ofGetHeight()*.5;
    
    lookAtCentroid.set(posX, posY);
    
    if (!oscReceiver.hasWaitingMessages()){
        for (int i=0; i<eyes.size(); i++){
            eyes[i]->rest();
        }
    }
    
    
    // check for waiting messages
    while (oscReceiver.hasWaitingMessages()){
        
        // get the next message
		ofxOscMessage m;
		oscReceiver.getNextMessage(&m);
        
        // check for position message
		if (m.getAddress() == "/controller/position"){
			
            posX = ofMap(m.getArgAsFloat(1), 0.0f, 100.0f, 0.0f, ofGetWidth());
            posY = ofMap(m.getArgAsFloat(0), 0.0f, 100.0f, 0.0f, ofGetHeight());
            
            lookAtCentroid.set(posX, posY);
            
            Tweener.addTween(debugCirclePos.x, lookAtCentroid.x, .1, &ofxTransitions::easeInOutSine);
            Tweener.addTween(debugCirclePos.y, lookAtCentroid.y, .1, &ofxTransitions::easeInOutSine);
            
            for (int i=0; i<eyes.size(); i++){
                eyes[i]->lookAt(lookAtCentroid);
            }
            
            string msg = "";
            msg += "position: " + ofToString(m.getArgAsFloat(0)) + ", " + ofToString(m.getArgAsFloat(1)) + "\n";
            msg += "look at: " + ofToString(posX) + ", " + ofToString(posY);
            
            ofLog(OF_LOG_NOTICE, msg);
		}
        
		// check for horizontal message
		else if (m.getAddress() == "/controller/horizontal"){
            
            eyeCountHorizontal = (int)m.getArgAsFloat(0);
            
            ofLog(OF_LOG_NOTICE, "eye count horizontal: " + ofToString(eyeCountHorizontal));
		}
        
        // check for vertical message
		else if (m.getAddress() == "/controller/vertical"){
			
            eyeCountVertical = (int)m.getArgAsFloat(0);
            
            ofLog(OF_LOG_NOTICE, "eye count vertical: " + ofToString(eyeCountVertical));
		}
        
        // check debug message
		else if (m.getAddress() == "/controller/debug"){
			bDebugMode = !(m.getArgAsFloat(0) == 0.0f);
		}
        
        // check for init message
		else if (m.getAddress() == "/controller/init"){
			bEyesInitialized = !(m.getArgAsFloat(0) == 0.0f);
            
            ofLog(OF_LOG_NOTICE, "initializing eyes...");
		}
	}
    
#endif
}

//--------------------------------------------------------------
void testApp::clearEyes(){
    if (eyes.size() > 0) {
        for (int i=0; i<eyes.size(); i++) {
            delete eyes[i];
        }
    }
    eyes.clear();
}

//--------------------------------------------------------------
void testApp::setupGui(){
    
    gui = new ofxUICanvas();
    gui->addLabel("OBSERVERS");
    gui->addSpacer();
    gui->addSlider("HORIZONTAL", 1.0f, MAX_EYES_HORIZONTAL, &eyeCountHorizontal);
    gui->addSlider("VERTICAL", 1.0f, MAX_EYES_VERTICAL, &eyeCountVertical);
    gui->addSpacer();
    gui->addLabelToggle("INIT", &bEyesInitialized);
    
    gui->addSpacer();
    gui->addFPSSlider("FPS");
    
#ifdef USE_BLOB_DETECTION
    vector<string> omNames;
    omNames.push_back("STATIC DIFFERENCE");
    omNames.push_back("CONTINIOUS DIFFERENCE");
    omNames.push_back("CONTINUOUS EASE DIFFERENCE");
    
    vector<string> colorModes;
    colorModes.push_back("RED");
    colorModes.push_back("GREEN");
    colorModes.push_back("BLUE");
    colorModes.push_back("LUMA601");
    colorModes.push_back("LUMA709");
    colorModes.push_back("LUMAUSER");
    
    vector<string> thresholdModes;
    thresholdModes.push_back("ABS DIFF");
    thresholdModes.push_back("LESSER");
    thresholdModes.push_back("GREATER");
    
    gui->autoSizeToFitWidgets();
    gui->addSlider("THRESHOLD", 0.0f, 80.0f, &threshold);
    gui->addSlider("FADE", 0.0f, 100.0f, &fade);
    
    ofxUIDropDownList *omList;
    ofxUIDropDownList *colorModeList;
    ofxUIDropDownList *thresholdModeList;
    
    omList = gui->addDropDownList("OM MODE", omNames);
    omList->setAllowMultiple(false);
    omList->setAutoClose(true);
    gui->addSpacer();
    
    colorModeList = gui->addDropDownList("COLOR MODE", colorModes);
    colorModeList->setAllowMultiple(false);
    colorModeList->setAutoClose(true);
    gui->addSpacer();
    
    thresholdModeList = gui->addDropDownList("THRESHOLD MODE", thresholdModes);
    thresholdModeList->setAllowMultiple(false);
    thresholdModeList->setAutoClose(true);
    gui->addSpacer();
    
    gui->addToggle("MIRROR X", true);
    gui->addSpacer();
    gui->addButton("CLEAR BACKGROUND", false);
    gui->addLabelToggle("DRAW FLOB", &bDrawFlob);
#endif
    
    gui->autoSizeToFitWidgets();
    
    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);
    
#ifdef USE_KINECT
    gui->loadSettings("GUI/guiSettingsKinect.xml");
#endif
#ifdef USE_BLOB_DETECTION
    gui->loadSettings("GUI/guiSettingsFlob.xml");
#endif
#ifdef USE_OSC
    gui->loadSettings("GUI/guiSettingsOSC.xml");
#endif
#ifdef USE_MOUSE
    gui->loadSettings("GUI/guiSettings.xml");
#endif
    gui->setVisible(true);
}

//--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e){

#ifdef USE_BLOB_DETECTION
    if (e.widget->getName() == "THRESHOLD")
        flob.setThresh(threshold);
    
    if (e.widget->getName() == "FADE")
        flob.setFade(fade);
    
    if (e.widget->getName() == "MIRROR X") {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        flob.mirrorX = toggle->getValue();
    }
    if (e.widget->getName() == "MIRROR Y") {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        flob.mirrorX = toggle->getValue();
    }
    
    if (e.widget->getName() == "CLEAR BACKGROUND")
        flob.clearBackground();
    
    if (e.widget->getName() == "OM MODE"){
        ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
        vector<ofxUIWidget *> &selected = ddlist->getSelected();
        for(int i = 0; i < selected.size(); i++) {
            if (selected[i]->getName() == "STATIC DIFFERENCE")
                flob.om = 0;
            else if (selected[i]->getName() == "CONTINIOUS DIFFERENCE")
                flob.om = 1;
            else if (selected[i]->getName() == "CONTINUOUS EASE DIFFERENCE")
                flob.om = 2;
        }
    }
    
    if (e.widget->getName() == "COLOR MODE"){
        ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
        vector<ofxUIWidget *> &selected = ddlist->getSelected();
        for(int i = 0; i < selected.size(); i++) {
            if (selected[i]->getName() == "RED")
                flob.colormode = 0;
            else if (selected[i]->getName() == "GREEN")
                flob.colormode = 1;
            else if (selected[i]->getName() == "BLUE")
                flob.colormode = 2;
            else if (selected[i]->getName() == "LUMA601")
                flob.colormode = 3;
            else if (selected[i]->getName() == "LUMA709")
                flob.colormode = 4;
            else if (selected[i]->getName() == "LUMAUSER")
                flob.colormode = 5;
        }
    }
    
    if (e.widget->getName() == "THRESHOLD MODE"){
        ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
        vector<ofxUIWidget *> &selected = ddlist->getSelected();
        for(int i = 0; i < selected.size(); i++) {
            if (selected[i]->getName() == "ABS DIFF")
                flob.thresholdmode = 0;
            else if (selected[i]->getName() == "LESSER")
                flob.thresholdmode = 1;
            else if (selected[i]->getName() == "GREATER")
                flob.thresholdmode = 2;
        }
    }
#endif
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key == 'd') {
        bDebugMode = !bDebugMode;
    }
}


//--------------------------------------------------------------
#ifdef USE_MOUSE
void testApp::mousePressed(int x, int y, int button){
    lookAtCentroid.set(x, y);
    
    Tweener.addTween(debugCirclePos.x, lookAtCentroid.x, .1, &ofxTransitions::easeInOutSine);
    Tweener.addTween(debugCirclePos.y, lookAtCentroid.y, .1, &ofxTransitions::easeInOutSine);
    
    for (int i=0; i<eyes.size(); i++){
        eyes[i]->lookAt(lookAtCentroid);
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    lookAtCentroid.set(x, y);
    
    Tweener.addTween(debugCirclePos.x, lookAtCentroid.x, .1, &ofxTransitions::easeInOutSine);
    Tweener.addTween(debugCirclePos.y, lookAtCentroid.y, .1, &ofxTransitions::easeInOutSine);
    
    for (int i=0; i<eyes.size(); i++){
        eyes[i]->rest();
    }
}
#endif


//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}


//--------------------------------------------------------------
void testApp::exit(){
    
    clearEyes();
    
#ifdef USE_KINECT
    gui->saveSettings("GUI/guiSettingsKinect.xml");
#endif
#ifdef USE_BLOB_DETECTION
    gui->saveSettings("GUI/guiSettingsFlob.xml");
#endif
#ifdef USE_OSC
    gui->saveSettings("GUI/guiSettingsOSC.xml");
#endif
#ifdef USE_MOUSE
    gui->saveSettings("GUI/guiSettings.xml");
#endif
    
    delete gui;
}
