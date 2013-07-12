#include "Eye.h"
#include "ofMain.h"

void Eye::setup(ofVec2f p, float w, float h)
{
	pos.set(p.x, p.y);
	
	width = w;
	height = h;
	
	centerPos.set(pos.x + width/2, pos.y + height/2);
	pupilPos.set(0, 0);
	
	dist	= 0;
	angle	= 0;
	
	size	= MIN(width, height);
	
	catchUpSpeed = .12f;
	
	surfaceImg.loadImage("surface1.png");
	whiteImg.loadImage("white1.png");
	pupilImg.loadImage("pupil1.png");
	shadeImg.loadImage("shade1.png");
    
    int numPixels			= pupilImg.width * pupilImg.height * 4;
	unsigned char * px		= pupilImg.getPixels();
	unsigned char * tarPx	= new unsigned char[pupilImg.width * pupilImg.height * 4];
	
	ofColor	*c = new ofColor;
	int hRand = (int) ofRandom(0, 200);
	int	sRand = (int) ofRandom(20, 100);
	
	for (int i=0; i<pupilImg.width; i ++)
	{
		for (int j=0; j<pupilImg.height; j++)
		{
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
    delete c;
	
	pupilImg.setFromPixels(tarPx, pupilImg.width, pupilImg.height, OF_IMAGE_COLOR_ALPHA);
	
	float psw = size * pupilImg.width / surfaceImg.width;
	float psh = size * pupilImg.height / surfaceImg.height;
	
	surfaceImg.resize(size, size);
	whiteImg.resize(size, size);
	shadeImg.resize(size, size);
	pupilImg.resize(psw, psh);
}

void Eye::update(){
	
}

//------------------------------------------------------------------
void Eye::xenoToPoint(float catchX, float catchY){
	pupilPos.x = catchUpSpeed * catchX + (1-catchUpSpeed) * pupilPos.x;
	pupilPos.y = catchUpSpeed * catchY + (1-catchUpSpeed) * pupilPos.y;
}

void Eye::draw(bool *debugMode){
	float cx = size/2;
	float cy = size/2;
	
	ofEnableAlphaBlending();
	ofSetColor(255, 255);
	if (*debugMode){
		ofNoFill();
		ofSetLineWidth(1);
		ofSetColor(100);
		ofEllipse(pos.x + size/2, pos.y + size/2, size * .8, size * .8);
		ofSetColor(255);
		
		ofPushMatrix();
		ofTranslate(centerPos.x, centerPos.y, 0);
		ofTranslate(pupilPos.x, pupilPos.x, 0);
		
		ofLine(-5, 0, 5, 0);
		ofLine(0, -5, 0, 5);
		
		ofPopMatrix();
		ofFill();
	} else {
        ofSetRectMode(OF_RECTMODE_CENTER);
		ofPushMatrix();
		ofTranslate(centerPos.x, centerPos.y, 0);
		whiteImg.draw(0, 0);
		pupilImg.draw(pupilPos.x, pupilPos.y);
		surfaceImg.draw(0, 0);
		shadeImg.draw(0, 0);
		ofPopMatrix();
		ofSetRectMode(OF_RECTMODE_CORNER);
	}
	ofDisableAlphaBlending();
}

void Eye::lookAt(ofVec2f *p){
    
	dist	= centerPos.distance(*p);
	//angle	= atan2(p->y - height/2, p->x - width/2);
	
	ofVec2f *pos = new ofVec2f;
	
	pos->set(p->x - centerPos.x, p->y - centerPos.y);
	
	pos->normalize();
	//*pos *= dist / 50;
	*pos *= size * .15;
	
	xenoToPoint(pos->x, pos->y);
}



