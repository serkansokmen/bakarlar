#pragma once

#include "ofMain.h"
#include "ofxTweener.h"


class Eye
{
	
	float eyeWidth;
	float eyeHeight;
    float eyeRadius;
	
	ofVec2f pupilPos;
	
public:
    
    void setup(const ofVec2f &p, float w, float h);
	void draw(bool *debugMode);
	
	void lookAt(const ofVec2f &p);
    void rest();
    
 	ofVec2f centerPos;
    
    ofImage surfaceImg;
	ofImage whiteImg;
	ofImage pupilImg;
	ofImage shadeImg;
};