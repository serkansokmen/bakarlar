#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxAnimatableOfPoint.h"
#include "ofxLibwebsockets.h"
#include "ofxXmlSettings.h"
#include "ofxDatGui.h"
#include "Grid.h"

class ofApp : public ofBaseApp {
    
public:
    void setup();
    void update();
    void draw();
    void drawPointCloud();
    void exit();
    
    void mousePressed(int x, int y, int button);
    void mouseMoved(int x, int y);
    void keyPressed(int key);
    void windowResized(int w, int h);
    
    void setupGui();
    void setupWebSocket();
    
    // websocket methods
    void onConnect( ofxLibwebsockets::Event& args );
    void onOpen( ofxLibwebsockets::Event& args );
    void onClose( ofxLibwebsockets::Event& args );
    void onIdle( ofxLibwebsockets::Event& args );
    void onMessage( ofxLibwebsockets::Event& args );
    void onBroadcast( ofxLibwebsockets::Event& args );
    
    void loadSettings();
    void saveSettings();
    
    ofxLibwebsockets::Client    client;
    eyegrid::Grid               eyeGrid;
    ofxOscReceiver              receiver;
    
    ofxDatGui*              gui;
    
    ofRectangle             gridRect;
    vector<ofVec2f>         poseVecs;
    
    bool bDrawGui;
};
