#include "Eye.h"
#include "ofMain.h"

void eye::Eye::setup(const ofVec2f &p, float w, float h)
{
    eyeWidth = w;
    eyeHeight = h;
    
    centerPos.set(p.x, p.y);
	
    eyeRadius = min(eyeWidth, eyeHeight);
    
    pupilPos.setDuration(.4);
    pupilPos.setPosition(centerPos);
    pupilPos.setRepeatType(PLAY_ONCE);
    pupilPos.setCurve(EASE_OUT);
    pupilPos.setRepeatTimes(0);
    
//    int numPixels			= pupilImg.getWidth() * pupilImg.getHeight() * 4;
//	ofPixels px             = pupilImg.getPixels();
	
//    // colorize pupil
//	ofColor	*c = new ofColor;
//	int hRand = (int) ofRandom(0, 200);
//	int	sRand = (int) ofRandom(20, 100);
//	
//	for (int i=0; i<pupilImg.width; i ++){
//		for (int j=0; j<pupilImg.height; j++){
//			int	index	= j * pupilImg.width + i;
//			
//			int r = px[index * 4 + 0];
//			int g = px[index * 4 + 1];
//			int b = px[index * 4 + 2];
//			int a = px[index * 4 + 3];
//			
//			c->r = r;
//			c->g = g;
//			c->b = b;
//			c->a = a;
//			
//			c->setHue(hRand);
//            c->setSaturation(sRand);
//            
//			tarPx[index * 4 + 0] = c->r;
//			tarPx[index * 4 + 1] = c->g;
//			tarPx[index * 4 + 2] = c->b;
//			tarPx[index * 4 + 3] = a;
//		}
//	}
//    
//	pupilImg.setFromPixels(tarPx, pupilImg.width, pupilImg.height, OF_IMAGE_COLOR_ALPHA);
	
    
	float pupilWidth = eyeRadius * pupilImg.getWidth() / surfaceImg.getHeight();
	float pupilHeight = eyeRadius * pupilImg.getWidth() / surfaceImg.getHeight();
    
    // resize images
    surfaceImg.resize(eyeRadius, eyeRadius);
    whiteImg.resize(eyeRadius, eyeRadius);
	shadeImg.resize(eyeRadius, eyeRadius);
	pupilImg.resize(pupilWidth, pupilHeight);
}

void eye::Eye::update(){
    float dt = 1.0f / 60.0f;
    pupilPos.update(dt);
}

void eye::Eye::draw(const bool& debugMode){
	
    float cx = eyeRadius/2;
	float cy = eyeRadius/2;
	
	if (debugMode){
        
        ofPushStyle();
        ofNoFill();
        ofSetColor(100);
        ofDrawCircle(centerPos + eyeRadius*.5, eyeRadius * .4f);
        
        ofPushMatrix();
        ofSetColor(255);
        ofTranslate(eyeRadius*.5 + pupilPos.getCurrentPosition());
        float length = eyeRadius * .1f;
        ofDrawLine(-length, 0, length, 0);
        ofDrawLine(0, length, 0, -length);
        ofPopMatrix();
        ofPopStyle();
        
        pupilPos.draw();
        
	} else {
        
        ofPushStyle();
        ofSetColor(255, 255);
		whiteImg.draw(centerPos);
        ofSetRectMode(OF_RECTMODE_CENTER);
		pupilImg.draw(eyeRadius*.5 + pupilPos.getCurrentPosition());
        ofSetRectMode(OF_RECTMODE_CORNER);
		surfaceImg.draw(centerPos);
		shadeImg.draw(centerPos);
        ofPopStyle();
        
	}
}

void eye::Eye::lookAt(const ofVec2f &p){
    float perc = ofNormalize(eyeRadius * .25, 0, ofGetHeight());
	ofVec2f v = centerPos.getInterpolated(p, perc);
    pupilPos.animateTo(v);
}

void eye::Eye::rest(){
    pupilPos.animateTo(centerPos);
}

