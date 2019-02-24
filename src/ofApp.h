#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOsc.h"
#include "ofxAnimatableOfPoint.h"
#include "ofxLibwebsockets.h"
#include "Grid.h"

// listening port
#define CLIENT_HOST "localhost"
#define CLIENT_PORT 8080
#define OSC_PORT    3000

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
    
    ofxLibwebsockets::Client client;
    
    // websocket methods
    void onConnect( ofxLibwebsockets::Event& args );
    void onOpen( ofxLibwebsockets::Event& args );
    void onClose( ofxLibwebsockets::Event& args );
    void onIdle( ofxLibwebsockets::Event& args );
    void onMessage( ofxLibwebsockets::Event& args );
    void onBroadcast( ofxLibwebsockets::Event& args );
    
    eyegrid::Grid                            eyeGrid;
    vector<shared_ptr<ofxAnimatableOfPoint>> lookAtPositions;
    vector<ofPoint>                          lastPositions;
    
    ofxOscReceiver receiver;
    
    ofxPanel                gui;
//    ofxDatGui               *datGui;
    
//    ofParameter<bool>       bUseGrabber;
//    ofParameter<bool>       bUsePlayer;
//    ofParameter<bool>       bUseKinect;
    ofRectangle             gridRect;
    
    bool bDrawGui;
};
