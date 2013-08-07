#pragma once

#include "ofMain.h"


class Eye
{
	
	float eyeWidth;
	float eyeHeight;
    float eyeRadius;
	
	ofVec2f pupilPos;
  	float   catchUpSpeed;
	
public:
    ~Eye();
    
    void setup(const ofVec2f &p, float w, float h);
	void update();
	void draw(bool *debugMode);
	
	void lookAt(const ofVec2f &p);
    void rest();
    
 	ofVec2f centerPos;
    
    ofImage surfaceImg;
	ofImage whiteImg;
	ofImage pupilImg;
	ofImage shadeImg;
};