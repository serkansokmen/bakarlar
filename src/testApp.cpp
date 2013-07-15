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
	
	eyeCountHorizontal	= 5;
	eyeCountVertical	= 5;
    minSize = MIN(ofGetWidth(), ofGetHeight());
    bDebugMode = true;
    bEyesInitialized = false;
    
    surfaceImg.loadImage("surface1.png");
	whiteImg.loadImage("white1.png");
	pupilImg.loadImage("pupil1.png");
	shadeImg.loadImage("shade1.png");
    
    threshold = 17.5f;
    fade = 27.0f;
    flob.mirrorX = true;
    flob.mirrorY = false;
    bDrawFlob = true;
	
	blobs = NULL;
	vidGrabber.setVerbose(true);
	vidGrabber.listDevices();
	vidGrabber.initGrabber(camWidth, camHeight);
	
    //	flob.setup((int)vidGrabber.getWidth(), (int)vidGrabber.getHeight(), ofGetWidth(), ofGetHeight());
	flob.setup(camWidth/2, camHeight/2, (float)ofGetWidth(), (float)ofGetHeight());
    
	flob.setOm(Flob::CONTINUOUS_DIFFERENCE)
        ->setColormode(Flob::LUMA601)
        ->setFade(fade)->setThresh(threshold)
        ->setThresholdmode(Flob::ABSDIF);
	
	cout << "ofxflob "  << flob.worldwidth << " : " << flob.worldheight << "\n ";
	cout << flob.videoresw << " : " << flob.videoresh << "\n ";
    
    setupGui();
}

//--------------------------------------------------------------
void testApp::setupGui(){
    
    gui = new ofxUICanvas();
    gui->addLabel("OBSERVERS");
    gui->addSpacer();
    gui->addSlider("HORIZONTAL", 1.0f, 100.0f, &eyeCountHorizontal);
    gui->addSlider("VERTICAL", 1.0f, 100.0f, &eyeCountVertical);
    gui->addLabelToggle("DEBUG MODE", &bDebugMode);
    
    gui->addSpacer();
    gui->addLabelToggle("INIT", &bEyesInitialized);
    
    gui->addSpacer();
    gui->addFPSSlider("FPS");
    
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
    
    gui->addToggle("MIRROR X", flob.getMirrorX());
    gui->addToggle("MIRROR Y", flob.getMirrorY());
    gui->addSpacer();
    gui->addButton("CLEAR BACKGROUND", false);
    gui->addLabelToggle("DRAW FLOB", &bDrawFlob);
    
    gui->autoSizeToFitWidgets();
    
    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);
    
    gui->loadSettings("GUI/guiSettings.xml");
    gui->setVisible(true);
}

//--------------------------------------------------------------
void testApp::update(){
    
    if (!bEyesInitialized){
		eyes.clear();
		initEyes();
		bEyesInitialized = true;
	} else {
		vidGrabber.update();
		
		if (vidGrabber.isFrameNew()){
            blobs = flob.calc(flob.binarize(vidGrabber.getPixels(), (int)vidGrabber.getWidth(), (int)vidGrabber.getHeight()));
		}
		
		
		for (int i=0; i<eyes.size(); i++){
            Eye *eye = eyes[i];
			eye->update();
            
            if (blobs != NULL && blobs->size() > 0){
                for (int n=0; n<blobs->size(); n++) {
                    ABlob &randomBlob = *(blobs->at(n));
                    
                    float x = ofMap(randomBlob.cx, 0, camWidth, 0, minSize);
                    float y = ofMap(randomBlob.cy, 0, camHeight, 0, minSize);
                    
                    ofVec2f	*pos = new ofVec2f;
                    pos->set(x, y);
                    eye->lookAt(pos);
                }
            }
		}
	}
}

//--------------------------------------------------------------
void testApp::draw(){

    if (bEyesInitialized)
	    for (int i=0; i<eyes.size(); i++) {
            ofPushMatrix();
            ofTranslate((ofGetWidth() - minSize)/2, 0);
            eyes[i]->draw(&bDebugMode);
            ofPopMatrix();
        }
    
    if (bDrawFlob) {
        
//        ofSetColor(255, 75);
//        flob.draw();
        
        int s = 128;
        flob.videoimggray.draw(ofGetWidth()-s, 0, s, s);
        flob.videotexbin.draw(ofGetWidth()-s, s, s, s);
        flob.videotexmotion.draw(ofGetWidth()-s, s+s, s, s);
        
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
	
    if (gui->isVisible()) gui->draw();
}

void testApp::mousePressed(int x, int y){

}

//--------------------------------------------------------------
void testApp::initEyes(){
    if (eyes.size() > 0) {
        for (int i=0; i<eyes.size(); i++) {
            delete eyes[i];
        }
        eyes.clear();
    }
    
    float eyeWidth	= minSize / (int)eyeCountHorizontal;
	float eyeHeight = minSize / (int)eyeCountVertical;
    
    for (int i = 0; i<(int)eyeCountHorizontal; i ++){
		for (int j = 0; j<(int)eyeCountVertical; j++){

			ofVec2f pos;
			pos.set(i*eyeWidth, j*eyeHeight);
            
			Eye * eye = new Eye;
            // Set pixel data before eye setup
			eye->surfaceImg.setFromPixels(surfaceImg.getPixelsRef());
            eye->whiteImg.setFromPixels(whiteImg.getPixelsRef());
            eye->pupilImg.setFromPixels(pupilImg.getPixelsRef());
            eye->shadeImg.setFromPixels(shadeImg.getPixelsRef());
            eye->setup(pos, eyeWidth, eyeHeight);
			eyes.push_back(eye);
            
            cout << "Added eye j:" << j << ", i:" << i << endl;
		}
	}
}

//--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e){

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
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key=='s') {
        gui->toggleVisible();
    }
}


//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}


//--------------------------------------------------------------
void testApp::exit(){
    gui->saveSettings("GUI/gui_settings.xml");
    delete gui;
}
