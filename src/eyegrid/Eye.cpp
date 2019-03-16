#include "Eye.h"
#include "ofMain.h"


using namespace eyegrid;

AnimCurve getRandomAnimCurve() {
    int rand = (int)ofRandom(0, 3);
    switch (rand) {
        case 0:
            return EASE_OUT;
            break;
        case 1:
            return LATE_EASE_IN_EASE_OUT;
            break;
        case 2:
            return VERY_LATE_LINEAR;
            break;
        default:
            return EASE_OUT_BACK;
            break;
    }
}

//--------------------------------------------------------------
void Eye::setup(const ofVec2f &p, float w, float h) {
    eyeRect.set(p, w, h);
    
    lookAtAnim.setPosition(eyeRect.getCenter());
    lookAtAnim.setRepeatType(PLAY_ONCE);
    lookAtAnim.setRepeatTimes(0);
    
    lastLookAt = 0;
    attentionDelay = (int)ofRandom(18, 200);
    
    restTimeDelta = EYE_REST_TIMEDELTA;
}

//--------------------------------------------------------------
void Eye::update() {
    actualTime = ofGetElapsedTimeMillis();
    float dt = 1.0f / 60.0f;
    lookAtAnim.update(dt);
    
    if (actualTime - restTimer > restTimeDelta) {
        lookAtAnim.animateTo(eyeRect.getCenter());
    }
}

//--------------------------------------------------------------
void Eye::draw(const bool& debugMode) {
	
    auto lookAt = lookAtAnim.getCurrentPosition();
    
	if (debugMode){
        
        float r = sqrt(eyeRect.getPerimeter());
        
        ofPushStyle();
        ofPushMatrix();
        ofSetColor(80);
        ofSetLineWidth(2);
        ofNoFill();
        ofDrawRectangle(eyeRect);
        ofDrawCircle(eyeRect.getCenter(), r * EYE_PERIPHERY_MULT);
        ofSetColor(150);
        ofTranslate(lookAt);
        ofDrawCircle(0, 0, r * EYE_PUPIL_SCL_MULT);

        float length = r * .1f;
        ofSetColor(255);
        ofDrawLine(-length, 0, length, 0);
        ofDrawLine(0, length, 0, -length);
        ofPopMatrix();
        ofPopStyle();
        
    } else if (whiteImg->isAllocated() &&
               pupilImg->isAllocated() &&
               surfaceImg->isAllocated() &&
               shadeImg -> isAllocated()) {
        
        ofPushMatrix();
        ofSetColor(255, 255);
        if (whiteImg->isAllocated() &&
            pupilImg->isAllocated() &&
            surfaceImg->isAllocated() &&
            shadeImg->isAllocated()) {
            
            whiteImg->draw(eyeRect);
            pupilImg->setAnchorPercent(0.5, 0.5);
            pupilImg->draw(lookAt,
                           eyeRect.getWidth() * EYE_PUPIL_SCL_MULT,
                           eyeRect.getHeight() * EYE_PUPIL_SCL_MULT);
            
            surfaceImg->draw(eyeRect);
            shadeImg->draw(eyeRect);
        }
        ofPopMatrix();
	}
}

void Eye::lookAt(const ofVec2f& vec) {
    
    if (lastLookAt <= attentionDelay) {
        lastLookAt++;
        return;
    }
    
    if (!lookAtAnim.isAnimating()) {
        lookAtAnim.setCurve(getRandomAnimCurve());
        lookAtAnim.setDuration(ofRandom(0.8) + 0.2);
        lastLookAt = 0;
        lookAtAnim.animateTo(vec);
    }
}

void Eye::rest() {
    restTimer = ofGetElapsedTimeMillis();
}
