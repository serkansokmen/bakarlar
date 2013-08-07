#include "Eye.h"
#include "ofMain.h"

void Eye::setup(const ofVec2f &p, float w, float h)
{
    eyeWidth = w;
    eyeHeight = h;
    
    centerPos.set(p.x, p.y);
	pupilPos.set(centerPos);
	
    eyeRadius = min(eyeWidth, eyeHeight);
	catchUpSpeed = .12f;
    
    int numPixels			= pupilImg.width * pupilImg.height * 4;
	unsigned char * px		= pupilImg.getPixels();
	unsigned char * tarPx	= new unsigned char[pupilImg.width * pupilImg.height * 4];
	
    // colorize pupil
	ofColor	*c = new ofColor;
	int hRand = (int) ofRandom(0, 200);
	int	sRand = (int) ofRandom(20, 100);
	
	for (int i=0; i<pupilImg.width; i ++){
		for (int j=0; j<pupilImg.height; j++){
			int	index	= j * pupilImg.width + i;
			
			int r = px[index * 4 + 0];
			int g = px[index * 4 + 1];
			int b = px[index * 4 + 2];
			int a = px[index * 4 + 3];
			
			c->r = r;
			c->g = g;
			c->b = b;
			c->a = a;
			
			c->setHue(hRand);
            c->setSaturation(sRand);
            
			tarPx[index * 4 + 0] = c->r;
			tarPx[index * 4 + 1] = c->g;
			tarPx[index * 4 + 2] = c->b;
			tarPx[index * 4 + 3] = a;
		}
	}
    
	pupilImg.setFromPixels(tarPx, pupilImg.width, pupilImg.height, OF_IMAGE_COLOR_ALPHA);
	
    
	float pupilWidth = eyeRadius * pupilImg.width / surfaceImg.width;
	float pupilHeight = eyeRadius * pupilImg.height / surfaceImg.height;
    
    // resize images
    surfaceImg.resize(eyeRadius, eyeRadius);
    whiteImg.resize(eyeRadius, eyeRadius);
	shadeImg.resize(eyeRadius, eyeRadius);
	pupilImg.resize(pupilWidth, pupilHeight);
    
    delete tarPx;
    delete c;
}

void Eye::update(){
    
}

void Eye::draw(bool *debugMode){
	
    float cx = eyeRadius/2;
	float cy = eyeRadius/2;
	
	if (*debugMode){
        
        ofPushStyle();
        ofNoFill();
        ofSetColor(100);
        ofCircle(centerPos, eyeRadius * .4f);
        
        ofSetColor(255);
        ofPushMatrix();
        ofTranslate(pupilPos);
        float length = eyeRadius * .1f;
        ofLine(-length, 0, length, 0);
        ofLine(0, length, 0, -length);
        ofPopMatrix();
//        ofDrawBitmapString(ofToString(pupilPos), pupilPos - eyeRadius * .1f);
		
        ofPopStyle();
        
	} else {
        ofSetRectMode(OF_RECTMODE_CENTER);

        ofPushStyle();
        ofSetColor(255, 255);
        
		whiteImg.draw(centerPos);
		pupilImg.draw(pupilPos.x, pupilPos.y);
		surfaceImg.draw(centerPos);
		shadeImg.draw(centerPos);
        
        ofPopStyle();
		ofSetRectMode(OF_RECTMODE_CORNER);
	}
}

void Eye::lookAt(const ofVec2f &p){
    
    float perc = ofNormalize(eyeRadius / 4, 0, ofGetHeight());
	ofVec2f v = centerPos.getInterpolated(p, perc);
    
    pupilPos.set(catchUpSpeed * v + (1-catchUpSpeed) * pupilPos);
}

void Eye::rest(){
    
    pupilPos.set(catchUpSpeed * centerPos + (1-catchUpSpeed) * pupilPos);
    pupilPos.set(centerPos);
}

Eye::~Eye() {
    
}

