#include "Eye.h"
#include "ofMain.h"


using namespace eyegrid;


//--------------------------------------------------------------
void Eye::setup(const ofVec2f &p, float w, float h)
{
    eyeWidth = w;
    eyeHeight = h;
    
    restPos.set(p.x, p.y);
	
    eyeRadius = min(eyeWidth, eyeHeight);
    
    pupilPos.setPosition(restPos);
    pupilPos.setRepeatType(PLAY_ONCE);
    pupilPos.setRepeatTimes(0);
    
//    pupilMtx.translate(restPos);
    
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
	pupilImg.resize(pupilWidth*0.85, pupilHeight*0.85);
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
        ofDrawCircle(0, 0, eyeRadius*EYE_PUPIL_MULT);

        float length = eyeRadius * .1f;
        ofSetColor(255);
        ofDrawLine(-length, 0, length, 0);
        ofDrawLine(0, length, 0, -length);
        ofPopMatrix();
        ofPopStyle();
        
	} else {
        
        ofPushStyle();
        ofSetColor(255, 255);
		whiteImg.draw(restPos);
        ofSetRectMode(OF_RECTMODE_CENTER);
		pupilImg.draw(pupilPos.getCurrentPosition() + eyeRadius*0.5);
        ofSetRectMode(OF_RECTMODE_CORNER);
		surfaceImg.draw(restPos);
		shadeImg.draw(restPos);
        ofPopStyle();
	}
}
