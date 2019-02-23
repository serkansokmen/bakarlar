#include "Eye.h"
#include "ofMain.h"


using namespace eyegrid;


//--------------------------------------------------------------
void Eye::setup(const ofVec2f &p, float w, float h)
{
    fbo.begin();
    ofClear(0, 0, 0, 0);
    fbo.end();
    
    eyeWidth = w;
    eyeHeight = h;
    
    restPos.set(p.x, p.y);
	
    eyeRadius = min(eyeWidth, eyeHeight);
    
    pupilPos.setPosition(restPos);
    pupilPos.setRepeatType(PLAY_ONCE);
    pupilPos.setRepeatTimes(0);
}

//--------------------------------------------------------------
void Eye::update(){
    float dt = 1.0f / 60.0f;
    pupilPos.update(dt);
}

//--------------------------------------------------------------
void Eye::draw(const bool& debugMode){
	
    float cx = eyeRadius/2;
	float cy = eyeRadius/2;
	
	if (debugMode){
        
        ofPushStyle();
        ofPushMatrix();
        ofTranslate(eyeRadius*0.5, eyeRadius*0.5);
        ofSetColor(80);
        ofNoFill();
        ofDrawCircle(restPos, eyeRadius*EYE_PERIPHERY_MULT);
        ofSetColor(150);
        ofTranslate(pupilPos.getCurrentPosition());
        ofDrawCircle(0, 0, eyeRadius*EYE_PUPIL_SCL_MULT);

        float length = eyeRadius * .1f;
        ofSetColor(255);
        ofDrawLine(-length, 0, length, 0);
        ofDrawLine(0, length, 0, -length);
        ofPopMatrix();
        ofPopStyle();
        
	} else {
        
        fbo.begin();
        ofSetColor(255, 255);
        if (whiteImg->isAllocated())
            whiteImg->draw(restPos, eyeWidth, eyeHeight);
        ofSetRectMode(OF_RECTMODE_CENTER);
        if (pupilImg->isAllocated())
            pupilImg->draw(pupilPos.getCurrentPosition() + eyeRadius*0.5,
                           eyeRadius*EYE_PUPIL_SCL_MULT,
                           eyeRadius*EYE_PUPIL_SCL_MULT);
        ofSetRectMode(OF_RECTMODE_CORNER);
        if (surfaceImg->isAllocated())
            surfaceImg->draw(restPos, eyeWidth, eyeHeight);
        if (shadeImg->isAllocated())
            shadeImg->draw(restPos, eyeWidth, eyeHeight);
        fbo.end();
        fbo.draw(restPos, eyeWidth, eyeHeight);
	}
}
