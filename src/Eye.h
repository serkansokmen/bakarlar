#pragma once

#include "ofMain.h"


class Eye
{
	
	float eyeWidth;
	float eyeHeight;
    float eyeRadius;
	
	ofVec2f pupilPos;
	
public:
    
    void setup(const ofVec2f& pos, float w, float h);
	void draw(bool *debugMode);
	
	void lookAt(const ofVec2f& pos, float gridSize);
    void rest();
    
 	ofVec2f centerPos;
    
    ofImage surfaceImg;
	ofImage whiteImg;
	ofImage pupilImg;
	ofImage shadeImg;
};