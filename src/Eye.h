#pragma once

#include "ofMain.h"

class Eye
{
	
	float width;
	float height;
	
	ofVec2f pupilPos;
	ofVec2f centerPos;
	
	float	dist;
	float	angle;
	float	size;
	
	ofImage surfaceImg;
	ofImage whiteImg;
	ofImage pupilImg;
	ofImage shadeImg;
	
public:
	
	void setup(ofVec2f p, float w, float h);
	void update();
	void updateWithRandomPos();
	void draw(bool *debugMode);
	
	void lookAt(ofVec2f *p);
	void xenoToPoint(float catchX, float catchY);
	
	float   catchUpSpeed;
	ofVec2f	pos;
};